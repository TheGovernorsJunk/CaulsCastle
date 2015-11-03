#include "mesh_manager.h"

#include "tmx.h"
#include "texture_manager.h"
#include "texture.h"
#include "mesh.h"

#include <vector>
#include <algorithm>

namespace te
{
    MeshManager::MeshManager(std::shared_ptr<const TMX> pTMX, std::shared_ptr<TextureManager> pTextureManager)
        : mpTMX(pTMX)
        , mpTextureManager(pTextureManager)
        , mMeshes()
    {
        //const TMX& tmx = *pTMX;
        //TextureManager& textureManager = *pTextureManager;

        //std::for_each(std::begin(tmx.layers), std::end(tmx.layers), [&, this](const TMX::Layer& layer) {
        //    if (layer.type != TMX::Layer::Type::OBJECTGROUP) { return; }

        //    std::for_each(std::begin(layer.objects), std::end(layer.objects), [&, this](const TMX::Tileset::Tile::ObjectGroup::Object& object) {
        //        if (mMeshes.find(object.gid) != mMeshes.end()) { return; }

        //        unsigned tilesetIndex = getTilesetIndex(tmx, object.gid);
        //        const TMX::Tileset& tileset = tmx.tilesets.at(tilesetIndex);
        //        std::shared_ptr<Texture> pTexture(textureManager[tmx.meta.path + "/" + tileset.image]);

        //        std::vector<Vertex> vertices(4);
        //        vertices[0].position = { 0, -object.height, 0 };
        //        vertices[1].position = { object.width, -object.height, 0 };
        //        vertices[2].position = { object.width, 0, 0 };
        //        vertices[3].position = { 0, 0, 0 };

        //        unsigned localIndex = object.gid - tileset.firstgid;

        //        unsigned sUnits = localIndex % ((tileset.imagewidth + tileset.spacing) / (tileset.tilewidth + tileset.spacing));
        //        unsigned sPixels = sUnits * (tileset.tilewidth + tileset.spacing);
        //        GLfloat s1 = (GLfloat)sPixels / (GLfloat)powerOfTwo(tileset.imagewidth);
        //        GLfloat s2 = (GLfloat)(sPixels + tileset.tilewidth) / (GLfloat)powerOfTwo(tileset.imagewidth);

        //        unsigned tUnits = localIndex / ((tileset.imagewidth + tileset.spacing) / (tileset.tilewidth + tileset.spacing));
        //        unsigned tPixels = tUnits * (tileset.tileheight + tileset.spacing);
        //        GLfloat t1 = (GLfloat)tPixels / (GLfloat)powerOfTwo(tileset.imageheight);
        //        GLfloat t2 = (GLfloat)(tPixels + tileset.tileheight) / (GLfloat)powerOfTwo(tileset.imageheight);

        //        vertices[0].texCoords = { s1, t1 };
        //        vertices[1].texCoords = { s2, t1 };
        //        vertices[2].texCoords = { s2, t2 };
        //        vertices[3].texCoords = { s1, t2 };

        //        std::vector<unsigned> indices(6);
        //        indices[0] = 0;
        //        indices[1] = 1;
        //        indices[2] = 2;
        //        indices[3] = 0;
        //        indices[4] = 2;
        //        indices[5] = 3;

        //        std::vector<std::shared_ptr<const Texture>> textures;
        //        textures.push_back(pTexture);

        //        mMeshes.insert(std::pair<unsigned, std::shared_ptr<Mesh>>{
        //            object.gid,
        //            std::shared_ptr<Mesh>(new Mesh(vertices, indices, textures))
        //        });
        //    });
        //});
    }

    std::shared_ptr<Mesh> MeshManager::operator[](unsigned gid)
    {
        auto it = mMeshes.find(gid);
        if (it != mMeshes.end()) {
            return it->second;
        }
        else {
            const TMX& tmx = *mpTMX;
            TextureManager& textureManager = *mpTextureManager;

            unsigned tilesetIndex = getTilesetIndex(tmx, gid);
            const TMX::Tileset& tileset = tmx.tilesets.at(tilesetIndex);
            std::shared_ptr<Texture> pTexture(textureManager[tmx.meta.path + "/" + tileset.image]);

            std::vector<Vertex> vertices(4);
            vertices[0].position = { 0, 0, 0 };
            vertices[1].position = { (float)tileset.tilewidth, 0, 0 };
            vertices[2].position = { (float)tileset.tilewidth, (float)tileset.tileheight, 0 };
            vertices[3].position = { 0, (float)tileset.tileheight, 0 };

            unsigned localIndex = gid - tileset.firstgid;

            unsigned sUnits = localIndex % ((tileset.imagewidth + tileset.spacing) / (tileset.tilewidth + tileset.spacing));
            unsigned sPixels = sUnits * (tileset.tilewidth + tileset.spacing);
            GLfloat s1 = (GLfloat)sPixels / (GLfloat)powerOfTwo(tileset.imagewidth);
            GLfloat s2 = (GLfloat)(sPixels + tileset.tilewidth) / (GLfloat)powerOfTwo(tileset.imagewidth);

            unsigned tUnits = localIndex / ((tileset.imagewidth + tileset.spacing) / (tileset.tilewidth + tileset.spacing));
            unsigned tPixels = tUnits * (tileset.tileheight + tileset.spacing);
            GLfloat t1 = (GLfloat)tPixels / (GLfloat)powerOfTwo(tileset.imageheight);
            GLfloat t2 = (GLfloat)(tPixels + tileset.tileheight) / (GLfloat)powerOfTwo(tileset.imageheight);

            vertices[0].texCoords = { s1, t1 };
            vertices[1].texCoords = { s2, t1 };
            vertices[2].texCoords = { s2, t2 };
            vertices[3].texCoords = { s1, t2 };

            std::vector<unsigned> indices(6);
            indices[0] = 0;
            indices[1] = 1;
            indices[2] = 2;
            indices[3] = 0;
            indices[4] = 2;
            indices[5] = 3;

            std::vector<std::shared_ptr<const Texture>> textures;
            textures.push_back(pTexture);

            std::shared_ptr<Mesh> pMesh(new Mesh(vertices, indices, textures));
            mMeshes.insert(std::pair<unsigned, std::shared_ptr<Mesh>>{
                gid,
                pMesh
            });

            return pMesh;
        }
    }
}
