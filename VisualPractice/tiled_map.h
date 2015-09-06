#ifndef TE_TILED_MAP_H
#define TE_TILED_MAP_H

#include "gl.h"
#include <glm/glm.hpp>

#include <string>
#include <memory>
#include <vector>
#include <map>

namespace te
{
    class Texture;
    struct BoundingBox;

    class TiledMap
    {
    public:
        TiledMap(
            const std::string& path,
            const std::string& filename,
            const glm::mat4& projection,
            const glm::mat4& model);

        TiledMap(TiledMap&& tm);
        TiledMap& operator=(TiledMap&& tm);

        ~TiledMap();

        void draw(const glm::mat4& viewTransform = glm::mat4()) const;
        bool checkCollision(const BoundingBox&) const;

    private:
        TiledMap(const TiledMap&) = delete;
        TiledMap& operator=(const TiledMap&) = delete;

        struct Tileset
        {
            std::shared_ptr<Texture> pTexture;
            GLint tileWidth;
            GLint tileHeight;
            GLint width;
            GLint height;
            unsigned firstGID;
        };

        struct Layer
        {
            GLuint vao;
            GLuint vbo;
            GLuint ebo;

            std::vector<unsigned> IDs;
            unsigned width;
            unsigned height;
        };

        GLuint mShaderProgram;
        std::vector<Tileset> mTilesets;
        std::vector<Layer> mLayers;
        std::map<unsigned, BoundingBox> mCollisionRects;
        glm::mat4 mModelMatrix;

        void destroy();
    };
}

#endif
