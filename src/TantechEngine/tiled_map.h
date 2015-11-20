#ifndef TE_TILED_MAP_H
#define TE_TILED_MAP_H

#include "tmx.h"

#include "gl.h"
#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace te
{
    class TextureManager;
    struct BoundingBox;
    class Mesh;
    class Model;
    class Shader;

    class TiledMap {
    public:
        TiledMap(const std::string& path, const std::string& file, std::shared_ptr<Shader> pShader, const glm::mat4& model, TextureManager* tm = nullptr);
        TiledMap(std::shared_ptr<const TMX> pTMX, std::shared_ptr<Shader> pShader, const glm::mat4& model, TextureManager* tm = nullptr);
        ~TiledMap();
        TiledMap(TiledMap&&);
        TiledMap& operator=(TiledMap&&);

        void draw(const glm::mat4& viewTransform = glm::mat4()) const;

        bool checkCollision(const BoundingBox&) const;
        bool checkCollision(const BoundingBox&, unsigned layerIndex) const;
        std::vector<BoundingBox>& getIntersections(const BoundingBox&, std::vector<BoundingBox>& intersections) const;
        std::vector<BoundingBox>& getIntersections(const BoundingBox&, unsigned layerIndex, std::vector<BoundingBox>& intersections) const;

        static void* operator new(std::size_t);
        static void operator delete(void*);
    private:
        TiledMap(const TiledMap&) = delete;
        TiledMap& operator=(const TiledMap&) = delete;

        void init(const TMX& tmx, TextureManager* tm);
        void destroy();
        bool checkUnitCollision(const BoundingBox& unitBB, const TMX::Layer& layer) const;
        void getUnitIntersections(const BoundingBox& unitBB, const TMX::Layer& layer, std::vector<BoundingBox>& intersections) const;

        glm::mat4 mModelMatrix;
        std::shared_ptr<Shader> mpShader;
        std::shared_ptr<const TMX> mpTMX;
        std::vector<Model> mLayers;
        std::map<unsigned, const BoundingBox> mCollisionRects;
    };
}

#endif
