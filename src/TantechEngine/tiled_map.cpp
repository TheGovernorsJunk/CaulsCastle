#include "tiled_map.h"
#include "tmx.h"
#include "shader.h"
#include "bounding_box_component.h"
#include "mesh.h"
#include "model.h"
#include "texture.h"
#include "texture_manager.h"
#include "auxiliary.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <algorithm>
#include <array>

namespace te
{
    TiledMap::TiledMap(const std::string& path, const std::string& file, std::shared_ptr<const Shader> pShader, const glm::mat4& model, TextureManager* tm)
        : mModelMatrix(model)
        , mpShader(pShader)
        , mpTMX(new TMX{path, file})
        , mLayers()
        , mCollisionRects()
    {
        init(*mpTMX, tm);
    }

    TiledMap::TiledMap(std::shared_ptr<const TMX> pTMX, std::shared_ptr<const Shader> pShader, const glm::mat4& model, TextureManager* tm)
        : mModelMatrix(model)
        , mpShader(pShader)
        , mpTMX(pTMX)
        , mLayers()
        , mCollisionRects()
    {
        init(*mpTMX, tm);
    }

    void TiledMap::init(const TMX& tmx, TextureManager* tm)
    {
        if (!mpShader) {
            throw std::runtime_error{ "TiledMap ctor: requires Shader." };
        }

        std::vector<std::shared_ptr<const Texture>> textures;
        std::for_each(std::begin(tmx.tilesets), std::end(tmx.tilesets), [&, this](const TMX::Tileset& tileset) {
            if (tm) {
                textures.push_back((*tm)[tileset]);
            } else {
                textures.push_back(std::shared_ptr<Texture>(new Texture{ tileset }));
            }

            std::for_each(std::begin(tileset.tiles), std::end(tileset.tiles), [&, this](const TMX::Tileset::Tile& tile) {
                std::for_each(std::begin(tile.objectGroup.objects), std::end(tile.objectGroup.objects), [&, this](const TMX::Tileset::Tile::ObjectGroup::Object& object) {
                    if (object.shape == TMX::Tileset::Tile::ObjectGroup::Object::Shape::RECTANGLE) {
                        mCollisionRects.insert(std::pair<unsigned, const BoundingBox>{
                            tileset.firstgid + tile.id,
                            {
                                object.x / tileset.tilewidth,
                                object.y / tileset.tileheight,
                                object.width / tileset.tilewidth,
                                object.height / tileset.tileheight
                            }
                        });
                    }
                });
            });
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

                int tilesetTextureIndex = getTilesetIndex(tmx, tileID);
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
                unsigned sPixels = sUnits * (tileset.tilewidth + tileset.spacing) + tileset.margin;
                GLfloat s1 = (GLfloat)sPixels / (GLfloat)powerOfTwo(tileset.imagewidth);
                GLfloat s2 = (GLfloat)(sPixels + tileset.tilewidth) / (GLfloat)powerOfTwo(tileset.imagewidth);

                unsigned tUnits = localIndex / ((tileset.imagewidth + tileset.spacing) / (tileset.tilewidth + tileset.spacing));
                unsigned tPixels = tUnits * (tileset.tileheight + tileset.spacing) + tileset.margin;
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

            std::vector<std::shared_ptr<const Mesh>> meshes;
            std::for_each(std::begin(protoMeshes), std::end(protoMeshes), [&meshes](ProtoMesh& proto) {
                if (proto.indices.size() > 0) {
                    meshes.push_back(std::shared_ptr<const Mesh>(new Mesh{ proto.vertices, proto.indices, proto.textures }));
                }
            });
            mLayers.push_back(Model{ std::move(meshes) });

            protoMeshes.clear();
            protoMeshes = std::vector<ProtoMesh>(tmx.tilesets.size());
        }

    }

    TiledMap::TiledMap(TiledMap&& o)
        : mpShader(std::move(o.mpShader))
        , mLayers(std::move(o.mLayers))
    {}

    TiledMap& TiledMap::operator=(TiledMap&& o)
    {
        destroy();

        mpShader = std::move(o.mpShader);
        mLayers = std::move(o.mLayers);

        return *this;
    }

    TiledMap::~TiledMap()
    {
        destroy();
    }

    void TiledMap::destroy()
    {
        mLayers.clear();
    }

    void TiledMap::draw(const glm::mat4& viewTransform) const
    {
        std::for_each(std::begin(mLayers), std::end(mLayers), [&, this](const Model& layer) {
            layer.draw(*mpShader, viewTransform * mModelMatrix);
        });
    }

    static unsigned getTileData(const TMX::Layer layer, int x, int y)
    {
        int index = y * layer.width + x;
        if (x < 0 || (unsigned)x >= layer.width || y < 0 || (unsigned)y >= layer.height || index < 0 || (unsigned)index >= layer.data.size()) {
            return 0;
        }
        return layer.data[index];
    }

    bool TiledMap::checkUnitCollision(const BoundingBox& unitBB, const TMX::Layer& layer) const
    {
        for (int x = (int)unitBB.x; x < (int)(unitBB.x + unitBB.w) + 1; ++x) {
            for (int y = (int)unitBB.y; y < (int)(unitBB.y + unitBB.h) + 1; ++y) {
                unsigned gid = getTileData(layer, x, y);

                if (gid == 0) { continue; }

                auto collisionRectIt = mCollisionRects.find(gid);
                if (collisionRectIt != mCollisionRects.end()) {
                    bool collision = te::checkCollision(
                        unitBB,
                        glm::translate(glm::vec3(x, y, 0)) * collisionRectIt->second);
                    if (collision) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    //bool TiledMap::checkCollision(const BoundingBox& worldBB) const
    //{
    //    BoundingBox unitBB = glm::scale(glm::vec3(1.f/mpTMX->tilewidth, 1.f/mpTMX->tileheight, 1)) *
    //        glm::inverse(mpShader->getModel()) * worldBB;

    //    bool collision = false;
    //    for (auto it = mpTMX->layers.begin(); it != mpTMX->layers.end(); ++it) {
    //        if (it->type != TMX::Layer::Type::TILELAYER) {
    //            continue;
    //        }
    //        collision = checkUnitCollision(unitBB, *it);
    //        if (collision) {
    //            break;
    //        }
    //    };
    //    return collision;
    //}

    //bool TiledMap::checkCollision(const BoundingBox& worldBB, unsigned layerIndex) const
    //{
    //    BoundingBox unitBB = glm::scale(glm::vec3(1.f/mpTMX->tilewidth, 1.f/mpTMX->tileheight, 1)) *
    //        glm::inverse(mpShader->getModel()) * worldBB;

    //    const TMX::Layer& layer = mpTMX->layers.at(layerIndex);
    //    if (layer.type != TMX::Layer::Type::TILELAYER) {
    //        throw std::runtime_error{ "getTileData: layer is not a tile data." };
    //    }

    //    return checkUnitCollision(unitBB, layer);
    //}

    //std::vector<BoundingBox>& TiledMap::getIntersections(const BoundingBox& worldBB, std::vector<BoundingBox>& bbs) const
    //{
    //    BoundingBox unitBB = glm::scale(glm::vec3(1.f/mpTMX->tilewidth, 1.f/mpTMX->tileheight, 1)) *
    //        glm::inverse(mpShader->getModel()) * worldBB;

    //    for (auto it = mpTMX->layers.begin(); it != mpTMX->layers.end(); ++it) {
    //        if (it->type != TMX::Layer::Type::TILELAYER) {
    //            continue;
    //        }
    //        getUnitIntersections(unitBB, *it, bbs);
    //    };

    //    return bbs;
    //}

    //std::vector<BoundingBox>& TiledMap::getIntersections(const BoundingBox& worldBB, unsigned layerIndex, std::vector<BoundingBox>& bbs) const
    //{
    //    BoundingBox unitBB = glm::scale(glm::vec3(1.f/mpTMX->tilewidth, 1.f/mpTMX->tileheight, 1)) *
    //        glm::inverse(mpShader->getModel()) * worldBB;

    //    const TMX::Layer& layer = mpTMX->layers.at(layerIndex);
    //    if (layer.type != TMX::Layer::Type::TILELAYER) {
    //        throw std::runtime_error{ "getTileData: layer is not a tile data." };
    //    }

    //    getUnitIntersections(unitBB, layer, bbs);
    //    return bbs;
    //}

    //void TiledMap::getUnitIntersections(const BoundingBox& unitBB, const TMX::Layer& layer, std::vector<BoundingBox>& bbs) const
    //{
    //    for (int x = (int)unitBB.x; x < (int)(unitBB.x + unitBB.w) + 1; ++x) {
    //        for (int y = (int)unitBB.y; y < (int)(unitBB.y + unitBB.h) + 1; ++y) {
    //            unsigned gid = getTileData(layer, x, y);

    //            if (gid == 0) { continue; }

    //            auto collisionRectIt = mCollisionRects.find(gid);
    //            if (collisionRectIt != mCollisionRects.end()) {
    //                BoundingBox translatedBB = glm::translate(glm::vec3(x, y, 0)) * collisionRectIt->second;
    //                if (te::checkCollision(unitBB, translatedBB)) {
    //                    bbs.push_back(mpShader->getModel() * glm::scale(glm::vec3(16.f, 16.f, 1.f)) * te::getIntersection(unitBB, translatedBB));
    //                }
    //            }
    //        }
    //    }
    //}

    void* TiledMap::operator new(std::size_t sz)
    {
        return _aligned_malloc(sz, 16);
    }

    void TiledMap::operator delete(void* ptr)
    {
        _aligned_free(ptr);
    }
}
