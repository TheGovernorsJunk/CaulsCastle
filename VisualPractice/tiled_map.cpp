#include "tiled_map.h"
#include "tmx.h"
#include "shader.h"
#include "bounding_box_component.h"
#include "mesh.h"
#include "texture.h"

#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <array>

namespace te
{
    TiledMap::TiledMap(const std::string& path, const std::string& file, const glm::mat4& projection, const glm::mat4& model)
        : mShaderProgram(loadProgram("tiled_map.glvs", "tiled_map.glfs"))
        , mModelMatrix(model)
        , mLayers()
    {
        init(path, TMX{ path, file }, projection, model);
    }

    TiledMap::TiledMap(const std::string& path, const TMX& tmx, const glm::mat4& projection, const glm::mat4& model)
    {
        init(path, tmx, projection, model);
    }

    void TiledMap::init(const std::string& path, const TMX& tmx, const glm::mat4& projection, const glm::mat4& model)
    {
        glUseProgram(mShaderProgram);

        GLint projectionMatrixLocation = glGetUniformLocation(mShaderProgram, "te_ProjectionMatrix");
        if (projectionMatrixLocation == -1) { throw std::runtime_error("te_ProjectionMatrix: not a valid program variable."); }
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection));

        GLint modelMatrixLocation = glGetUniformLocation(mShaderProgram, "te_ModelMatrix");
        if (modelMatrixLocation == -1) { throw std::runtime_error("te_ModelMatrix: not a valid program variable."); }
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));

        // TODO: Get from an asset manager
        std::vector<std::shared_ptr<const Texture>> textures;
        std::for_each(std::begin(tmx.tilesets), std::end(tmx.tilesets), [&textures, &path](const TMX::Tileset& tileset) {
            textures.push_back(std::shared_ptr<const Texture>(new Texture{ path + "/" + tileset.image }));
        });

        struct ProtoMesh {
            std::vector<Vertex> vertices;
            std::vector<GLuint> indices;
            std::vector<std::shared_ptr<const Texture>> textures;
            unsigned elementIndex;
            ProtoMesh() : vertices(), indices(), textures(), elementIndex(0) {}
        };
        std::vector<ProtoMesh> protoMeshes(tmx.tilesets.size());

        for (auto it = tmx.layers.begin(); it != tmx.layers.end(); ++it) {
            const TMX::Layer& layer = *it;
            unsigned layerIndex = it - tmx.layers.begin();

            for (auto it = layer.data.begin(); it != layer.data.end(); ++it) {

                unsigned tileID = *it;
                if (tileID == 0) { continue; }

                std::array<Vertex, 4> corners{};

                int tilesetTextureIndex = getTilesetTextureIndex(tmx, tileID);
                const TMX::Tileset& tileset = tmx.tilesets.at(tilesetTextureIndex);

                unsigned i = it - layer.data.begin();

                unsigned xUnit = tileset.tilewidth;
                float x = (float)((i % layer.width) * xUnit);

                unsigned yUnit = tileset.tileheight;
                float y = (float)((i / layer.width) * yUnit);

                corners[0].position = { x, y, (float)layerIndex };
                corners[1].position = { x + xUnit, y, (float)layerIndex };
                corners[2].position = { x + xUnit, y + yUnit, (float)layerIndex };
                corners[3].position = { x, y + yUnit, (float)layerIndex };

                unsigned localIndex = tileID - tileset.firstgid;

                unsigned sUnits = localIndex % ((tileset.imagewidth + tileset.spacing) / (tileset.tilewidth + tileset.spacing));
                unsigned sPixels = sUnits * (tileset.tilewidth + tileset.spacing);
                GLfloat s1 = (GLfloat)sPixels / (GLfloat)powerOfTwo(tileset.imagewidth);
                GLfloat s2 = (GLfloat)(sPixels + tileset.tilewidth) / (GLfloat)powerOfTwo(tileset.imagewidth);

                unsigned tUnits = localIndex / ((tileset.imagewidth + tileset.spacing) / (tileset.tilewidth + tileset.spacing));
                unsigned tPixels = tUnits * (tileset.tileheight + tileset.spacing);
                GLfloat t1 = (GLfloat)tPixels / (GLfloat)powerOfTwo(tileset.imageheight);
                GLfloat t2 = (GLfloat)(tPixels + tileset.tileheight) / (GLfloat)powerOfTwo(tileset.imageheight);

                corners[0].texCoords = { s1, t1 };
                corners[1].texCoords = { s2, t1 };
                corners[2].texCoords = { s2, t2 };
                corners[3].texCoords = { s1, t2 };

                assert(s1 < 1.f && t1 < 1.f);

                ProtoMesh& currMesh = protoMeshes.at(tilesetTextureIndex);
                std::for_each(std::begin(corners), std::end(corners), [&currMesh](Vertex& vertex) {
                    currMesh.vertices.push_back(std::move(vertex));
                });

                currMesh.indices.push_back(currMesh.elementIndex * 4);
                currMesh.indices.push_back(currMesh.elementIndex * 4 + 1);
                currMesh.indices.push_back(currMesh.elementIndex * 4 + 2);
                currMesh.indices.push_back(currMesh.elementIndex * 4);
                currMesh.indices.push_back(currMesh.elementIndex * 4 + 2);
                currMesh.indices.push_back(currMesh.elementIndex * 4 + 3);
                ++currMesh.elementIndex;
            }

            for (auto it = protoMeshes.begin(); it != protoMeshes.end(); ++it) {
                it->textures.push_back(textures.at(it - protoMeshes.begin()));
            }

            std::vector<std::shared_ptr<Mesh>> meshes;
            std::for_each(std::begin(protoMeshes), std::end(protoMeshes), [&meshes](ProtoMesh& proto) {
                if (proto.indices.size() > 0) {
                    meshes.push_back(std::shared_ptr<Mesh>(new Mesh{ proto.vertices, proto.indices, proto.textures }));
                }
            });
            mLayers.push_back(Layer{ std::move(meshes) });

            protoMeshes.clear();
            protoMeshes = std::vector<ProtoMesh>(tmx.tilesets.size());
        }

    }

    TiledMap::TiledMap(TiledMap&& o)
        : mShaderProgram(std::move(o.mShaderProgram))
        , mModelMatrix(o.mModelMatrix)
        , mLayers(std::move(o.mLayers))
    {
        o.mShaderProgram = 0;
    }

    TiledMap& TiledMap::operator=(TiledMap&& o)
    {
        destroy();

        mShaderProgram = std::move(o.mShaderProgram);
        mModelMatrix = std::move(o.mModelMatrix);
        mLayers = std::move(o.mLayers);

        o.mShaderProgram = 0;

        return *this;
    }

    TiledMap::~TiledMap()
    {
        destroy();
    }

    void TiledMap::destroy()
    {
        glDeleteProgram(mShaderProgram);
        mShaderProgram = 0;
        mLayers.clear();
    }

    // TODO: Robust implementation
    int TiledMap::getTilesetTextureIndex(const TMX& tmx, unsigned tileID) const
    {
        for (auto it = tmx.tilesets.begin(); it != tmx.tilesets.end(); ++it) {
            unsigned firstInclusive = it->firstgid;
            unsigned lastExclusive = it->firstgid + it->tilecount;

            if ((tileID >= firstInclusive) && (tileID < lastExclusive)) {
                return it - tmx.tilesets.begin();
            }
        }
        throw std::out_of_range("No tileset for given tile ID.");
    }

    void TiledMap::draw(const glm::mat4& viewTransform) const
    {
        glUseProgram(mShaderProgram);

        GLint viewMatrixLocation = glGetUniformLocation(mShaderProgram, "te_ViewMatrix");
        if (viewMatrixLocation == -1) { throw std::runtime_error("te_ViewMatrix: not a valid program variable."); }
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewTransform));

        std::for_each(std::begin(mLayers), std::end(mLayers), [this](const Layer& layer) {
            std::for_each(std::begin(layer.meshes), std::end(layer.meshes), [this](std::shared_ptr<Mesh> mesh) {

                // Currently only one usable texture
                glActiveTexture(GL_TEXTURE0);
                glUniform1i(glGetUniformLocation(mShaderProgram, "base"), 0);
                glBindTexture(GL_TEXTURE_2D, mesh->getTexture(0)->getID());

                glBindVertexArray(mesh->getVAO());
                glDrawElements(GL_TRIANGLES, mesh->getElementCount(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            });
        });
    }

    bool TiledMap::checkCollision(const BoundingBox& worldBB) const
    {
        return false;
    }

    std::vector<BoundingBox> TiledMap::getIntersections(const BoundingBox&) const
    {
        return{};
    }
}
