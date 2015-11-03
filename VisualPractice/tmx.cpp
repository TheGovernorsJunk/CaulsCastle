#include "tmx.h"
#include "entity_manager.h"
#include "transform_component.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <glm/gtx/transform.hpp>

#include <map>
#include <memory>
#include <functional>

namespace te
{
    static TMX::Tileset::Tile::ObjectGroup::Object::Shape getShapeEnum(const std::string& shapeStr)
    {
        static const std::map<std::string, TMX::Tileset::Tile::ObjectGroup::Object::Shape> shapeMap = {
            {"rectangle", TMX::Tileset::Tile::ObjectGroup::Object::Shape::RECTANGLE}
        };
        try {
            return shapeMap.at(shapeStr);
        }
        catch (std::out_of_range e) {
            throw std::runtime_error("Unsupported shape type.");
        }
    }

    static TMX::Layer::Type getLayerType(const std::string& layerStr)
    {
        static const std::map<std::string, TMX::Layer::Type> layerMap = {
            {"tilelayer", TMX::Layer::Type::TILELAYER},
            {"objectgroup", TMX::Layer::Type::OBJECTGROUP}
        };
        try {
            return layerMap.at(layerStr);
        }
        catch (std::out_of_range e) {
            throw std::runtime_error("Unsupported layer type.");
        }
    }

    static TMX::Orientation getOrientation(const std::string& orientationStr)
    {
        static const std::map<std::string, TMX::Orientation> orientationMap = {
            {"orthogonal", TMX::Orientation::ORTHOGONAL}
        };
        try {
            return orientationMap.at(orientationStr);
        }
        catch (std::out_of_range e) {
            throw std::runtime_error("Unsupported orientation.");
        }
    }

    static TMX::RenderOrder getRenderOrder(const std::string& renderOrderStr)
    {
        static const std::map<std::string, TMX::RenderOrder> renderOrderMap = {
            {"right-down", TMX::RenderOrder::RIGHT_DOWN}
        };
        try {
            return renderOrderMap.at(renderOrderStr);
        }
        catch (std::out_of_range e) {
            throw std::runtime_error("Unsupported render order.");
        }
    }

    static luabridge::LuaRef getTMXRef(lua_State *L, const std::string& path, const std::string& filename)
    {
        luaL_openlibs(L);
        int status = luaL_dofile(L, "map_loader.lua");

        if (status) { throw std::runtime_error("Could not load script."); }

        return luabridge::LuaRef(luabridge::getGlobal(L, "loadMap")(std::string{ path + "/" + filename }.c_str()));
    }

    TMX::Meta::Meta(std::string path, std::string file)
        : path(path), file(file) {}

    TMX::TMX(const std::string& path, const std::string& file)
        : meta(path, file)
    {
        std::unique_ptr<lua_State, std::function<void(lua_State*)>> L(
            luaL_newstate(),
            [](lua_State* L) {lua_close(L); });
        luabridge::LuaRef tmx = getTMXRef(L.get(), path, file);

        orientation = getOrientation(tmx["orientation"]);
        renderorder = getRenderOrder(tmx["renderorder"]);
        width = tmx["width"];
        height = tmx["height"];
        tilewidth = tmx["tilewidth"];
        tileheight = tmx["tileheight"];
        nextobjectid = tmx["nextobjectid"];

        // tilesets initialization
        {
            luabridge::LuaRef tilesetsRef = tmx["tilesets"];
            for (int i = 1; !tilesetsRef[i].isNil(); ++i) {
                luabridge::LuaRef tilesetRef = tilesetsRef[i];
                luabridge::LuaRef tileoffsetRef = tilesetRef["tileoffset"];

                TMX::Tileset tileset{
                    tilesetRef["name"],
                    tilesetRef["firstgid"],
                    tilesetRef["tilewidth"],
                    tilesetRef["tileheight"],
                    tilesetRef["spacing"],
                    tilesetRef["margin"],
                    tilesetRef["image"],
                    tilesetRef["imagewidth"],
                    tilesetRef["imageheight"],
                    {tileoffsetRef["x"], tileoffsetRef["y"]},
                    std::vector<TMX::Tileset::Terrain>(),
                    tilesetRef["tilecount"],
                    std::vector<TMX::Tileset::Tile>()
                };

                // terrains initialization
                {
                    luabridge::LuaRef terrainsRef = tilesetRef["terrains"];
                    for (int i = 1; !terrainsRef[i].isNil(); ++i) {
                        luabridge::LuaRef terrainRef = terrainsRef[i];

                        TMX::Tileset::Terrain terrain{
                            terrainRef["name"],
                            terrainRef["tile"]
                        };
                        tileset.terrains.push_back(std::move(terrain));
                    }
                } // end terrains initialization

                // tiles initialization
                {
                    luabridge::LuaRef tilesRef = tilesetRef["tiles"];
                    for (int i = 1; !tilesRef[i].isNil(); ++i) {
                        luabridge::LuaRef tileRef = tilesRef[i];

                        // objectGroup initialization
                        luabridge::LuaRef objectGroupRef = tileRef["objectGroup"];
                        TMX::Tileset::Tile::ObjectGroup objectGroup{};
                        if (objectGroupRef.isNil()) {
                            objectGroup.type = TMX::Tileset::Tile::ObjectGroup::Type::NONE;
                        }
                        else if (objectGroupRef["type"] == "objectgroup") {
                            objectGroup.type = TMX::Tileset::Tile::ObjectGroup::Type::OBJECTGROUP;
                            objectGroup.name = objectGroupRef["name"].cast<std::string>();
                            objectGroup.visible = objectGroupRef["visible"];
                            objectGroup.opacity = objectGroupRef["opacity"];
                            objectGroup.offsetx = objectGroupRef["offsetx"];
                            objectGroup.offsety = objectGroupRef["offsety"];

                            // objects initialization
                            {
                                luabridge::LuaRef objectsRef = objectGroupRef["objects"];
                                for (int i = 1; !objectsRef[i].isNil(); ++i) {
                                    luabridge::LuaRef objectRef = objectsRef[i];

                                    luabridge::LuaRef gidRef = objectRef["gid"];
                                    TMX::Tileset::Tile::ObjectGroup::Object object{
                                        objectRef["id"],
                                        objectRef["name"],
                                        objectRef["type"],
                                        getShapeEnum(objectRef["shape"]),
                                        objectRef["x"],
                                        objectRef["y"],
                                        objectRef["width"],
                                        objectRef["height"],
                                        objectRef["rotation"],
                                        gidRef.isNil() ? 0 : gidRef.cast<unsigned>(),
                                        objectRef["visible"]
                                    };

                                    objectGroup.objects.push_back(std::move(object));
                                }
                            } // end objects initialization
                        }
                        // end objectGroup initialization

                        TMX::Tileset::Tile tile{
                            tileRef["id"],
                            std::map<std::string, std::string>{},
                            std::move(objectGroup),
                            std::vector<TMX::Tileset::Tile::Frame>{},
                            std::vector<unsigned>{},
                        };

                        // properties initialization
                        {
                            luabridge::LuaRef propertiesRef = tileRef["properties"];
                            if (!propertiesRef.isNil()) {
                                for (luabridge::Iterator it(propertiesRef); !it.isNil(); ++it) {
                                    luabridge::LuaRef key = it.key();
                                    luabridge::LuaRef val = *it;
                                    tile.properties.insert(std::pair<std::string, std::string>{
                                        key,
                                        val
                                    });
                                }
                            }
                        } // end properties initialization

                        // animation initialization
                        {
                            luabridge::LuaRef animationRef = tileRef["animation"];
                            for (int i = 1; !animationRef.isNil() && !animationRef[i].isNil(); ++i) {
                                luabridge::LuaRef frameRef = animationRef[i];

                                TMX::Tileset::Tile::Frame frame{
                                    frameRef["tileid"],
                                    frameRef["duration"]
                                };

                                tile.animation.push_back(std::move(frame));
                            }
                        } // end animation initialization

                        // terrain initialization
                        {
                            luabridge::LuaRef terrainRef = tileRef["terrain"];
                            for (int i = 1; !terrainRef.isNil() && !terrainRef[i].isNil(); ++i) {
                                tile.terrain.push_back(terrainRef[i]);
                            }
                        } // end terrain initialization

                        tileset.tiles.push_back(std::move(tile));
                    }
                } // end tiles initialization

                tilesets.push_back(std::move(tileset));
            }
        } // end tilesets initialization

        // layers initialization
        {
            luabridge::LuaRef layersRef = tmx["layers"];
            for (int i = 1; !layersRef.isNil() && !layersRef[i].isNil(); ++i) {
                luabridge::LuaRef layerRef = layersRef[i];

                TMX::Layer::Type type = getLayerType(layerRef["type"]);

                TMX::Layer layer{
                    type,
                    layerRef["name"],
                    type == TMX::Layer::Type::OBJECTGROUP ? 0 : layerRef["x"],
                    type == TMX::Layer::Type::OBJECTGROUP ? 0 : layerRef["y"],
                    type == TMX::Layer::Type::OBJECTGROUP ? 0 : layerRef["width"],
                    type == TMX::Layer::Type::OBJECTGROUP ? 0 : layerRef["height"],
                    layerRef["visible"],
                    layerRef["opacity"],
                    layerRef["offsetx"],
                    layerRef["offsety"]
                };

                // data initialization
                {
                    luabridge::LuaRef dataRef = layerRef["data"];
                    for (int i = 1; !dataRef.isNil() && !dataRef[i].isNil(); ++i) {
                        layer.data.push_back(dataRef[i]);
                    }
                } // end data initialization

                // objects initialization
                {
                    luabridge::LuaRef objectsRef = layerRef["objects"];
                    for (int i = 1; !objectsRef.isNil() && !objectsRef[i].isNil(); ++i) {
                        luabridge::LuaRef objectRef = objectsRef[i];

                        luabridge::LuaRef gidRef = objectRef["gid"];
                        TMX::Tileset::Tile::ObjectGroup::Object object{
                            objectRef["id"],
                            objectRef["name"],
                            objectRef["type"],
                            getShapeEnum(objectRef["shape"]),
                            objectRef["x"],
                            objectRef["y"],
                            objectRef["width"],
                            objectRef["height"],
                            objectRef["rotation"],
                            gidRef.isNil() ? 0 : gidRef.cast<unsigned>(),
                            objectRef["visible"]
                        };

                        layer.objects.push_back(std::move(object));
                    }
                }

                layers.push_back(std::move(layer));
            }
        } // end layers initialization
    } // end TMX constructor

    unsigned getTilesetIndex(const TMX& tmx, unsigned gid)
    {
        for (auto it = tmx.tilesets.begin(); it != tmx.tilesets.end(); ++it) {
            unsigned firstInclusive = it->firstgid;
            unsigned lastExclusive = it->firstgid + it->tilecount;

            if ((gid >= firstInclusive) && (gid < lastExclusive)) {
                return it - tmx.tilesets.begin();
            }
        }
        throw std::out_of_range("No tileset for given tile ID.");
    }
}
