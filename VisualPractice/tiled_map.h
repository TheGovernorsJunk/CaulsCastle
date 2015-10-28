#ifndef TE_TILED_MAP_H
#define TE_TILED_MAP_H

#include "texture.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>
#include <memory>

namespace te
{
    struct BoundingBox;

    struct TMX {
        TMX(const std::string& path, const std::string& file);

        struct Meta {
            std::string path;
            std::string file;
            Meta(std::string p, std::string f);
        } meta;

        enum class Orientation {
            ORTHOGONAL
        } orientation;
        enum class RenderOrder {
            RIGHT_DOWN
        } renderorder;

        unsigned width;
        unsigned height;
        unsigned tilewidth;
        unsigned tileheight;
        unsigned nextobjectid;

        struct Tileset {
            std::string name;
            unsigned firstgid;
            unsigned tilewidth;
            unsigned tileheight;
            int spacing;
            int margin;
            std::string image;
            unsigned imagewidth;
            unsigned imageheight;

            struct Tileoffset {
                int x;
                int y;
            } tileoffset;

            struct Terrain {
                std::string name;
                int tile;
            };
            std::vector<Terrain> terrains;

            unsigned tilecount;

            struct Tile {
                unsigned id;

                struct ObjectGroup {

                    enum class Type {
                        OBJECTGROUP,
                        NONE
                    } type;
                    std::string name;
                    bool visible;
                    float opacity;
                    int offsetx;
                    int offsety;

                    struct Object {
                        unsigned id;
                        std::string name;
                        std::string type;
                        enum class Shape {
                            RECTANGLE
                        } shape;
                        int x;
                        int y;
                        unsigned width;
                        unsigned height;
                        float rotation;
                        bool visible;
                    };
                    std::vector<Object> objects;
                } objectGroup;

                struct Frame {
                    unsigned tileid;
                    unsigned duration;
                };
                std::vector<Frame> animation;

                std::vector<unsigned> terrain;
            };
            std::vector<Tile> tiles;
        };
        std::vector<Tileset> tilesets;

        struct Layer {
            enum class Type {
                TILELAYER,
                OBJECTGROUP
            } type;
            std::string name;
            int x;
            int y;
            unsigned width;
            unsigned height;
            bool visible;
            float opacity;
            int offsetx;
            int offsety;
            std::vector<unsigned> data;
            std::vector<Tileset::Tile::ObjectGroup::Object> objects;
        };
        std::vector<Layer> layers;
    };

    class TiledMap {
    public:
        TiledMap(const std::string& path, const std::string& file, const glm::mat4& projection, const glm::mat4& model);
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
            GLuint vao;
            GLuint vbo;
            GLuint ebo;
            GLsizei elementCount;
        };

        int getTilesetTextureIndex(unsigned tileID) const;
        void destroy();

        TMX mTMX;
        GLuint mShaderProgram;
        std::vector<std::shared_ptr<Texture>> mTilesetTextures;
        std::vector<Layer> mLayers;
    };
}

#endif













////#ifndef TE_TILED_MAP_H
////#define TE_TILED_MAP_H
////
//#include "gl.h"
//#include <glm/glm.hpp>
//
//#include <string>
//#include <memory>
//#include <vector>
//#include <map>
//#include <functional>
//
//namespace te
//{
//    class Texture;
//    struct BoundingBox;
//
//    class TiledMap
//    {
//    public:
//        TiledMap(
//            const std::string& path,
//            const std::string& filename,
//            const glm::mat4& projection,
//            const glm::mat4& model);
//
//        TiledMap(TiledMap&& tm);
//        TiledMap& operator=(TiledMap&& tm);
//
//        ~TiledMap();
//
//        void draw(const glm::mat4& viewTransform = glm::mat4()) const;
//        bool checkCollision(const BoundingBox&) const;
//        std::vector<BoundingBox> getIntersections(const BoundingBox&) const;
//
//    private:
//        TiledMap(const TiledMap&) = delete;
//        TiledMap& operator=(const TiledMap&) = delete;
//
//        struct Tileset
//        {
//            std::shared_ptr<Texture> pTexture;
//            GLint tileWidth;
//            GLint tileHeight;
//            GLint width;
//            GLint height;
//            unsigned firstGID;
//            int spacing;
//        };
//
//        struct Layer
//        {
//            GLuint vao;
//            GLuint vbo;
//            GLuint ebo;
//
//            std::vector<unsigned> IDs;
//            unsigned width;
//            unsigned height;
//        };
//
//        GLuint mShaderProgram;
//        std::vector<Tileset> mTilesets;
//        std::vector<Layer> mLayers;
//        std::map<unsigned, BoundingBox> mCollisionRects;
//        glm::mat4 mModelMatrix;
//
//        void destroy();
//        void applyToOverlaps(const BoundingBox&, std::function<bool(const BoundingBox&, const Layer&, unsigned tileIndex, unsigned x, unsigned y)>) const;
//    };
//}
////
////#endif
