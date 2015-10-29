#ifndef TE_TILED_MAP_H
#define TE_TILED_MAP_H

#include "gl.h"
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <memory>

namespace te
{
    class TextureManager;
    struct TMX;
    struct BoundingBox;
    class Mesh;

    class TiledMap {
    public:
        TiledMap(const std::string& path, const std::string& file, const glm::mat4& projection, const glm::mat4& model, TextureManager* tm = nullptr);
        TiledMap(const std::string& path, const TMX& tmx, const glm::mat4& projection, const glm::mat4& model, TextureManager* tm = nullptr);
        ~TiledMap();
        TiledMap(TiledMap&&);
        TiledMap& operator=(TiledMap&&);

        void draw(const glm::mat4& viewTransform = glm::mat4()) const;
        bool checkCollision(const BoundingBox&) const;
        std::vector<BoundingBox> getIntersections(const BoundingBox&) const;

    private:
        TiledMap(const TiledMap&) = delete;
        TiledMap& operator=(const TiledMap&) = delete;

        struct Layer {
            std::vector<std::shared_ptr<Mesh>> meshes;
        };

        void init(const std::string& path, const TMX& tmx, const glm::mat4& projection, const glm::mat4& model, TextureManager* tm);
        int getTilesetTextureIndex(const TMX& tmx, unsigned tileID) const;
        void destroy();

        GLuint mShaderProgram;
        glm::mat4 mModelMatrix;
        std::vector<Layer> mLayers;
    };
}

#endif
