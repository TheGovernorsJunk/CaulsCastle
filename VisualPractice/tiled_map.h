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
    class Shader;

    class TiledMap {
    public:
        TiledMap(const std::string& path, const std::string& file, std::shared_ptr<Shader> pShader, TextureManager* tm = nullptr);
        TiledMap(const TMX& tmx, std::shared_ptr<Shader> pShader, TextureManager* tm = nullptr);
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

        void init(const TMX& tmx, TextureManager* tm);
        void destroy();

        std::shared_ptr<Shader> mpShader;
        std::vector<Layer> mLayers;
    };
}

#endif
