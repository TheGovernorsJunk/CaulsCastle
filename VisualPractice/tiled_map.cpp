#include "tiled_map.h"
#include "shader.h"
#include "bounding_box_component.h"

#include <glm/gtc/type_ptr.hpp>
//#include <glm/gtx/transform.hpp>
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

                        layer.objects.push_back(std::move(object));
                    }
                }

                layers.push_back(std::move(layer));
            }
        } // end layers initialization
    } // end TMX constructor

    TiledMap::TiledMap(const std::string& path, const std::string& file, const glm::mat4& projection, const glm::mat4& model)
        : mTMX(path, file)
        , mShaderProgram(loadProgram("tiled_map.glvs", "tiled_map.glfs"))
        , mModelMatrix(model)
        , mTilesetTextures()
        , mLayers()
    {
        glUseProgram(mShaderProgram);

        GLint projectionMatrixLocation = glGetUniformLocation(mShaderProgram, "te_ProjectionMatrix");
        if (projectionMatrixLocation == -1) { throw std::runtime_error("te_ProjectionMatrix: not a valid program variable."); }
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projection));

        GLint modelMatrixLocation = glGetUniformLocation(mShaderProgram, "te_ModelMatrix");
        if (modelMatrixLocation == -1) { throw std::runtime_error("te_ModelMatrix: not a valid program variable."); }
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(model));

        std::for_each(std::begin(mTMX.tilesets), std::end(mTMX.tilesets), [this](TMX::Tileset& tileset) {
            mTilesetTextures.push_back(std::shared_ptr<Texture>{new Texture{ mTMX.meta.path + "/" + tileset.image }});
        });

        struct Vertex {
            struct Position {
                GLfloat x;
                GLfloat y;
                GLfloat z;
            } position;
            struct TexCoords {
                GLfloat s;
                GLfloat t;
            } texCoords;
            GLint sampler;
        };
        std::vector<Vertex> vertices;

        std::vector<GLuint> indices;

        unsigned elementIndex = 0;
        std::for_each(std::begin(mTMX.layers), std::end(mTMX.layers), [&vertices, &indices, &elementIndex, this](TMX::Layer& layer) {

            for (auto it = layer.data.begin(); it != layer.data.end(); ++it) {

                unsigned tileID = *it;
                if (tileID == 0) { continue; }

                std::array<Vertex, 4> corners{};

                unsigned i = it - layer.data.begin();
                float x = (float)(i % layer.width);
                float y = (float)(i / layer.width);
                corners[0].position = { x, y, 0 };
                corners[1].position = { x + 1, y, 0 };
                corners[2].position = { x + 1, y + 1, 0 };
                corners[3].position = { x, y + 1, 0 };

                int tilesetTextureIndex = getTilesetTextureIndex(tileID);
                const TMX::Tileset& tileset = mTMX.tilesets.at(tilesetTextureIndex);
                const Texture& texture = *mTilesetTextures.at(tilesetTextureIndex);

                unsigned localIndex = tileID - tileset.firstgid;

                unsigned sUnits = localIndex % ((tileset.imagewidth + tileset.spacing) / (tileset.tilewidth + tileset.spacing));
                unsigned sPixels = sUnits * (tileset.tilewidth + tileset.spacing);
                GLfloat s1 = (GLfloat)sPixels / (GLfloat)texture.getTexWidth();
                GLfloat s2 = (GLfloat)(sPixels + tileset.tilewidth) / (GLfloat)texture.getTexWidth();

                unsigned tUnits = localIndex / ((tileset.imagewidth + tileset.spacing) / (tileset.tilewidth + tileset.spacing));
                unsigned tPixels = tUnits * (tileset.tileheight + tileset.spacing);
                GLfloat t1 = (GLfloat)tPixels / (GLfloat)texture.getTexHeight();
                GLfloat t2 = (GLfloat)(tPixels + tileset.tileheight) / (GLfloat)texture.getTexHeight();

                corners[0].texCoords = { s1, t1 };
                corners[1].texCoords = { s2, t1 };
                corners[2].texCoords = { s2, t2 };
                corners[3].texCoords = { s1, t2 };

                assert(s1 < 1.f && t1 < 1.f);

                std::for_each(std::begin(corners), std::end(corners), [tilesetTextureIndex](Vertex& corner) {
                    corner.sampler = tilesetTextureIndex;
                });

                std::for_each(std::begin(corners), std::end(corners), [&vertices](Vertex& vertex) {
                    vertices.push_back(std::move(vertex));
                });

                indices.push_back(elementIndex * 4);
                indices.push_back(elementIndex * 4 + 1);
                indices.push_back(elementIndex * 4 + 2);
                indices.push_back(elementIndex * 4);
                indices.push_back(elementIndex * 4 + 2);
                indices.push_back(elementIndex * 4 + 3);
                ++elementIndex;
            }

            GLuint vao = 0, vbo = 0, ebo = 0;
            glGenVertexArrays(1, &vao);
            glBindVertexArray(vao);

            glGenBuffers(1, &vbo);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 1, GL_INT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, sampler));
            glEnableVertexAttribArray(2);

            for (auto it = mTilesetTextures.begin(); it != mTilesetTextures.end(); ++it)
            {
                unsigned textureIndex = it - mTilesetTextures.begin();
                glActiveTexture(GL_TEXTURE0 + textureIndex);
                glBindTexture(GL_TEXTURE_2D, it->get()->getID());
            }

            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);

            mLayers.push_back({ vao, vbo, ebo, indices.size() });

            vertices.clear();
            indices.clear();
            elementIndex = 0;
        });

    }

    TiledMap::TiledMap(TiledMap&& o)
        : mTMX(std::move(o.mTMX))
        , mShaderProgram(std::move(o.mShaderProgram))
        , mModelMatrix(o.mModelMatrix)
        , mTilesetTextures(std::move(o.mTilesetTextures))
        , mLayers(std::move(o.mLayers))
    {
        o.mShaderProgram = 0;
    }

    TiledMap& TiledMap::operator=(TiledMap&& o)
    {
        destroy();

        mTMX = std::move(o.mTMX);
        mShaderProgram = std::move(o.mShaderProgram);
        mModelMatrix = std::move(o.mModelMatrix);
        mTilesetTextures = std::move(o.mTilesetTextures);
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
        std::for_each(std::begin(mLayers), std::end(mLayers), [](Layer& buffers)
        {
            glDeleteBuffers(1, &buffers.vbo);
            glDeleteBuffers(1, &buffers.ebo);
            glDeleteVertexArrays(1, &buffers.vao);
        });
        glDeleteProgram(mShaderProgram);

        mShaderProgram = 0;
        mTilesetTextures.clear();
        mLayers.clear();
    }

    // TODO: Robust implementation
    int TiledMap::getTilesetTextureIndex(unsigned tileID) const
    {
        for (auto it = mTMX.tilesets.begin(); it != mTMX.tilesets.end(); ++it) {
            unsigned firstInclusive = it->firstgid;
            unsigned lastExclusive = it->firstgid + it->tilecount;

            if ((tileID >= firstInclusive) && (tileID < lastExclusive)) {
                return it - mTMX.tilesets.begin();
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

        for (auto it = mTilesetTextures.begin(); it != mTilesetTextures.end(); ++it) {
            unsigned textureIndex = it - mTilesetTextures.begin();
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            glBindTexture(GL_TEXTURE_2D, it->get()->getID());
            std::stringstream ss;
            ss << textureIndex;
            std::string textureIndexStr;
            ss >> textureIndexStr;
            glUniform1i(glGetUniformLocation(mShaderProgram, std::string("samplers[" + textureIndexStr + "]").c_str()), textureIndex);
        }
        std::for_each(std::begin(mLayers), std::end(mLayers), [](const Layer& layer) {
            glBindVertexArray(layer.vao);
            glDrawElements(GL_TRIANGLES, layer.elementCount, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
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
