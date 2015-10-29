#include "tiled_map.h"
#include "shader.h"
#include "bounding_box_component.h"
#include "mesh.h"

#include <glm/gtc/type_ptr.hpp>
#include <lua.hpp>
#include <LuaBridge.h>

#include <functional>
#include <map>
#include <algorithm>
#include <array>

namespace te
{
    static TMX::Tileset::Tile::ObjectGroup::Object::Shape getShapeEnum(const std::string& shapeStr)
    {
        static const std::map<std::string, TMX::Tileset::Tile::ObjectGroup::Object::Shape> shapeMap = {
            {"rectangle", TMX::Tileset::Tile::ObjectGroup::Object::Shape::RECTANGLE}
        };
        try {
            return shapeMap.at(shapeStr);
        } catch(std::out_of_range e) {
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
        } catch (std::out_of_range e) {
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
        } catch (std::out_of_range e) {
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
            [](lua_State* L){lua_close(L); });
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
                                        objectRef["visible"]
                                    };

                                    objectGroup.objects.push_back(std::move(object));
                                }
                            } // end objects initialization
                        }
                        // end objectGroup initialization

                        TMX::Tileset::Tile tile{
                            tileRef["id"],
                            std::move(objectGroup),
                            std::vector<TMX::Tileset::Tile::Frame>{},
                            std::vector<unsigned>{},
                        };

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

                unsigned i = it - layer.data.begin();
                float x = (float)(i % layer.width);
                float y = (float)(i / layer.width);
                corners[0].position = { x, y, (float)layerIndex };
                corners[1].position = { x + 1, y, (float)layerIndex };
                corners[2].position = { x + 1, y + 1, (float)layerIndex };
                corners[3].position = { x, y + 1, (float)layerIndex };

                int tilesetTextureIndex = getTilesetTextureIndex(tmx, tileID);
                const TMX::Tileset& tileset = tmx.tilesets.at(tilesetTextureIndex);

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
