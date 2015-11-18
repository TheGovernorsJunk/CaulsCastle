#ifndef TE_TMX_H
#define TE_TMX_H

#include "gl.h"

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace te
{
    struct TMX {
        TMX(const std::string& path, const std::string& file);
        TMX(const std::string& pathfile);

        struct Meta {
            std::string path;
            std::string file;
            Meta(const std::string& p, const std::string& f);
            Meta(const std::string& pf);
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
            struct TransparentColor {
                GLubyte r;
                GLubyte g;
                GLubyte b;
                bool inUse;
            } transparentcolor;

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

                std::map<std::string, std::string> properties;

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
                        float x;
                        float y;
                        float width;
                        float height;
                        float rotation;
                        unsigned gid;
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

    unsigned getTilesetIndex(const TMX& tmx, unsigned gid);

    class BadFilename : public std::runtime_error {
    public:
        BadFilename(const char* message);
    };

    class Shader;
    class MeshManager;
    class EntityManager;
    class TransformComponent;
    class AnimationComponent;

    void loadObjects(
        std::shared_ptr<const TMX>,
        const Shader& shader,
        std::shared_ptr<MeshManager>,
        EntityManager&,
        TransformComponent&,
        AnimationComponent&);
}

#endif
