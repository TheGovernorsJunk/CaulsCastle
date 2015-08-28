#include "tiled_map.h"

#include "shader.h"
#include "texture.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <lua.hpp>
#include <LuaBridge.h>

#include <functional>
#include <array>

namespace te
{
    TiledMap::TiledMap(const std::string& path, const std::string& filename)
        : mShaderProgram(loadProgram("tiled_map.glvs", "tiled_map.glfs")), mTilesets(), mBuffers()
    {
        glUseProgram(mShaderProgram);

        GLint projectionMatrixLocation = glGetUniformLocation(mShaderProgram, "te_ProjectionMatrix");
        if (projectionMatrixLocation == -1) { throw std::runtime_error("te_ProjectionMatrix: not a valid program variable."); }
        glm::mat4 projectionMatrix = glm::ortho<GLfloat>(0.0, 16, 9, 0.0, 1.0, -1.0);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        GLint modelViewMatrixLocation = glGetUniformLocation(mShaderProgram, "te_ModelViewMatrix");
        if (modelViewMatrixLocation == -1) { throw std::runtime_error("te_ModelViewMatrix: not a valid program variable."); }
        glm::mat4 modelViewMatrix;
        glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

        std::unique_ptr<lua_State, std::function<void(lua_State*)>> L(
            luaL_newstate(),
            [](lua_State* L){lua_close(L); });
        luaL_openlibs(L.get());
        int status = luaL_dofile(L.get(), "map_loader.lua");

        if (status) { throw std::runtime_error("Could not load script."); }

        luabridge::LuaRef tiledData(luabridge::getGlobal(L.get(), "loadMap")(std::string{ path + "/" + filename }.c_str()));

        luabridge::LuaRef tilesets = tiledData["tilesets"];
        for (int i = 1; !tilesets[i].isNil(); ++i)
        {
            luabridge::LuaRef tileset = tilesets[i];
            mTilesets.push_back({
                std::shared_ptr < Texture > {new Texture{ path + "/" + tileset["image"].cast<std::string>() }},
                tileset["tilewidth"].cast<GLint>(),
                tileset["tileheight"].cast<GLint>(),
                tileset["imagewidth"].cast<GLint>(),
                tileset["imageheight"].cast<GLint>(),
                tileset["firstgid"].cast<int>()
            });
        }

        struct Vertex
        {
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

        luabridge::LuaRef layers = tiledData["layers"];
        for (int l = 1; !layers[l].isNil(); ++l)
        {
            luabridge::LuaRef layer = layers[l];
            int layerWidth = layer["width"].cast<int>();
            int layerHeight = layer["height"].cast<int>();

            std::vector<Vertex> vertices;
            vertices.reserve(layerWidth * layerHeight * 4);
            std::vector<GLuint> indices;
            indices.reserve(layerWidth * layerHeight * 6);

            luabridge::LuaRef data = layer["data"];
            for (int t = 0; !data[t + 1].isNil(); ++t)
            {
                std::array<Vertex, 4> vertex;

                GLfloat x = (GLfloat)(t % layerWidth);
                GLfloat y = (GLfloat)(t / layerWidth);

                vertex[0].position = { x, y, (GLfloat)((l - 1) * 100) };
                vertex[1].position = { x + 1, y, (GLfloat)((l - 1) * 100) };
                vertex[2].position = { x + 1, y + 1, (GLfloat)((l - 1) * 100) };
                vertex[3].position = { x, y + 1, (GLfloat)((l - 1) * 100) };

                unsigned tileIndex = data[t + 1].cast<unsigned>();
                Tileset* pTileset = 0;
                for (auto it = mTilesets.begin(); it != mTilesets.end(); ++it)
                {
                    if (it->firstGID <= tileIndex && ((it + 1)->firstGID > tileIndex || (it + 1) == mTilesets.end()))
                    {
                        for (int i = 0; i < 4; ++i)
                            vertex[i].sampler = it - mTilesets.begin();
                        pTileset = &*it;
                    }
                }

                unsigned tileOffset = tileIndex - pTileset->firstGID;
                GLint tileWidth = pTileset->tileWidth;
                GLint tileHeight = pTileset->tileHeight;
                GLint ts = (tileOffset * tileWidth) % pTileset->width;
                GLint tt = ((tileOffset * tileHeight) / pTileset->width) * tileHeight;
                GLfloat tilesetWidth = (GLfloat)pTileset->pTexture->getTexWidth();
                GLfloat tilesetHeight = (GLfloat)pTileset->pTexture->getTexHeight();

                vertex[0].texCoords = { ts / tilesetWidth, tt / tilesetHeight };
                vertex[1].texCoords = { (ts + tileWidth) / tilesetWidth, tt / tilesetHeight };
                vertex[2].texCoords = { (ts + tileWidth) / tilesetWidth, (tt + tileHeight) / tilesetHeight };
                vertex[3].texCoords = { ts / tilesetWidth, (tt + tileHeight) / tilesetHeight };

                std::for_each(std::begin(vertex), std::end(vertex), [&vertices](Vertex& v)
                {
                    vertices.push_back(std::move(v));
                });

                indices.push_back(t * 4);
                indices.push_back(t * 4 + 1);
                indices.push_back(t * 4 + 2);
                indices.push_back(t * 4);
                indices.push_back(t * 4 + 2);
                indices.push_back(t * 4 + 3);
            }

            GLuint vao, vbo, ebo;
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

            for (auto it = mTilesets.begin(); it != mTilesets.end(); ++it)
            {
                unsigned textureIndex = it - mTilesets.begin();
                glActiveTexture(GL_TEXTURE0 + textureIndex);
                glBindTexture(GL_TEXTURE_2D, it->pTexture->getID());
            }

            glGenBuffers(1, &ebo);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);

            mBuffers.push_back({ vao, vbo, ebo });
        }
    }

    TiledMap::TiledMap(TiledMap&& tm)
        : mShaderProgram(tm.mShaderProgram)
        , mTilesets(std::move(tm.mTilesets))
        , mBuffers(std::move(tm.mBuffers))
    {
        tm.mShaderProgram = 0;
    }

    TiledMap& TiledMap::operator=(TiledMap&& tm)
    {
        destroy();

        mShaderProgram = tm.mShaderProgram;
        mTilesets = std::move(tm.mTilesets);
        mBuffers = std::move(tm.mBuffers);

        tm.mShaderProgram = 0;

        return *this;
    }

    TiledMap::~TiledMap()
    {
        destroy();
    }

    void TiledMap::draw(const glm::mat4& viewTransform) const
    {
        glUseProgram(mShaderProgram);
        glUniformMatrix4fv(
            glGetUniformLocation(mShaderProgram, "te_ModelViewMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(viewTransform));

        for (auto it = mTilesets.begin(); it != mTilesets.end(); ++it)
        {
            unsigned textureIndex = it - mTilesets.begin();
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            glBindTexture(GL_TEXTURE_2D, it->pTexture->getID());
            std::stringstream ss;
            ss << textureIndex;
            std::string textureIndexStr;
            ss >> textureIndexStr;
            glUniform1i(glGetUniformLocation(mShaderProgram, std::string("samplers[" + textureIndexStr + "]").c_str()), textureIndex);
        }
        std::for_each(std::begin(mBuffers), std::end(mBuffers), [](const Buffers& buffers)
        {
            glBindVertexArray(buffers.vao);
            glDrawElements(GL_TRIANGLES, 1800, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        });
    }

    void TiledMap::destroy()
    {
        std::for_each(std::begin(mBuffers), std::end(mBuffers), [](Buffers& buffers)
        {
            glDeleteBuffers(1, &buffers.vbo);
            glDeleteBuffers(1, &buffers.ebo);
            glDeleteVertexArrays(1, &buffers.vao);
        });
        glDeleteProgram(mShaderProgram);

        mShaderProgram = 0;
        mTilesets.clear();
        mBuffers.clear();
    }
}
