#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <lua.hpp>
#include <LuaBridge.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <bass.h>
#include "types.h"
#include "wrappers.h"
#include "auxiliary.h"
#include "player.h"
#include "entity_manager.h"
#include "transform_component.h"
#include "physics_component.h"
#include "physics_system.h"
#include "bounding_box_component.h"
#include "collision_system.h"
#include "simple_render_component.h"
#include "render_system.h"
#include "texture.h"
#include "shader.h"

using namespace te;

namespace te
{
    struct Sprite
    {
        TexturePtr pTexture;
        int w;
        int h;
        Sprite(TexturePtr pTexture, int w, int h)
            : pTexture(pTexture), w(w), h(h) {}
    };

    class CollisionHandler : public Observer<CollisionEvent>
    {
    public:
        typedef std::pair<Entity, Entity> EntityPair;

        std::map<EntityPair, std::function<void(Entity, Entity, float)>> map;

        virtual void onNotify(const CollisionEvent& evt)
        {
            EntityPair pair = { evt.a, evt.b };
            auto it = map.find(pair);
            if (it != map.end())
            {
                it->second(evt.a, evt.b, evt.dt);
            }
        }
    };

    class LuaGameState
    {
    public:
        LuaGameState(const TransformationData& tData, const std::string& filename = "init.lua")
            : mEntities()
            , mpTransformComponent(new TransformComponent())
            , mpPhysicsComponent(new PhysicsComponent())
            , mpBoundingBoxComponent(new BoundingBoxComponent(mpTransformComponent))
            , mpRenderComponent(new SimpleRenderComponent())
            , mEntityManager({mpTransformComponent, mpPhysicsComponent, mpBoundingBoxComponent, mpRenderComponent})
            , mPhysicsSystem(mpPhysicsComponent, mpTransformComponent)
            , mCollisionSystem(mpBoundingBoxComponent, {&mCollisionHandler})
            , mTransformationData(tData)
            , mRenderSystem(mpRenderComponent, mpTransformComponent, mTransformationData)
            , mKeyPressTable()
            , mKeyReleaseTable()
            , mCollisionHandler()
            , mFontCount(0)
            , mFontMap()
            , mChannel(0)
        {
            if (!(mChannel = BASS_StreamCreateFile(false, "sounds/arcade-beep-1.mp3", 0, 0, 0)))
            {
                throw std::runtime_error("Could not load sound.");
            }
        }

        typedef unsigned int EntityHandle;
        typedef std::pair<Entity, Entity> EntityPair;
        typedef luabridge::LuaRef LuaVector;
        typedef luabridge::LuaRef LuaFunction;
        typedef unsigned int FontHandle;
        typedef unsigned int TextHandle;

        void playSound()
        {
            BASS_ChannelPlay(mChannel, true);
        }

        Entity createEntity(glm::vec2 position, glm::vec2 velocity = glm::vec2(0, 0))
        {
            Entity entity = mEntityManager.create();
            mEntities.push_back(entity);
            mpTransformComponent->setLocalTransform(
                entity,
                glm::translate(glm::vec3(position.x, position.y, 0.f)));
            mpPhysicsComponent->setPhysics(entity, velocity);
            return entity;
        }

        void destroyEntity(Entity entity)
        {
            mEntityManager.destroy(entity);
        }

        void setPosition(Entity entity, glm::vec2 position)
        {
            if (!exists(entity)) return;

            mpTransformComponent->setLocalTransform(
                entity,
                glm::translate(glm::vec3(position.x, position.y, 0)));
        }

        glm::vec2 getPosition(Entity entity)
        {
            if (!exists(entity)) return glm::vec2(0.f, 0.f);

            glm::mat4 localTransform = mpTransformComponent->getLocalTransform(entity);
            glm::vec4 position = localTransform * glm::vec4(0.f, 0.f, 0.f, 1.f);

            return glm::vec2(position.x, position.y);
        }

        void setVelocity(Entity entity, glm::vec2 velocity)
        {
            if (!exists(entity)) return;

            mpPhysicsComponent->setPhysics(entity, velocity);
        }

        glm::vec2 getVelocity(Entity entity)
        {
            return mpPhysicsComponent->getVelocity(entity);
        }

        void setBoundingBox(Entity entity, glm::vec2 dimensions)
        {
            if (!exists(entity)) return;

            mpBoundingBoxComponent->setBoundingBox(entity, dimensions, { 0, 0 });
        }

        void setSprite(Entity entity, glm::vec2 dimensions)
        {
            mpRenderComponent->setSprite(entity, dimensions, { 0, 0 });
        }

        void scale(Entity entity, glm::vec3 scale)
        {
            mpTransformComponent->setLocalTransform(
                entity,
                glm::scale(mpTransformComponent->getLocalTransform(entity), scale));
        }

        void setParent(Entity child, Entity parent)
        {
            mpTransformComponent->setParent(child, parent);
        }

        void handleCollision(Entity e1, Entity e2, std::function<void(Entity,Entity,float)> handler)
        {
            if (!exists(e1) || !exists(e2)) return;

            auto key = std::make_pair(e1, e2);
            auto it = mCollisionHandler.map.find(key);
            if (it == mCollisionHandler.map.end())
            {
                mCollisionHandler.map.insert(std::make_pair(
                    key,
                    handler));
            }
            else
            {
                it->second = handler;
            }
        }

        void registerKeyPress(char character, std::function<void(void)> func)
        {
            auto it = mKeyPressTable.find(character);
            if (it == mKeyPressTable.end())
            {
                mKeyPressTable.insert(std::make_pair(character, func));
            }
            else
            {
                mKeyPressTable[character] = func;
            }
        }

        void registerKeyRelease(char character, std::function<void(void)> func)
        {
            auto it = mKeyReleaseTable.find(character);
            if (it == mKeyReleaseTable.end())
            {
                mKeyReleaseTable.insert(std::make_pair(character, func));
            }
            else
            {
                mKeyReleaseTable[character] = func;
            }
        }

        FontHandle loadFont(const std::string& filename, int ptSize)
        {
            FontPtr pFont = te::loadFont(filename, ptSize);

            mFontMap.insert(std::make_pair(
                mFontCount,
                pFont));

            return mFontCount++;
        }

        bool exists(Entity entity)
        {
            return mEntityManager.isAlive(entity);
        }

        void processInput(const SDL_Event& evt)
        {
            if (evt.type == SDL_KEYDOWN)
            {
                auto it = mKeyPressTable.find(evt.key.keysym.sym);
                if (it != mKeyPressTable.end())
                {
                    it->second();
                }
            }
            else if (evt.type == SDL_KEYUP)
            {
                auto it = mKeyReleaseTable.find(evt.key.keysym.sym);
                if (it != mKeyReleaseTable.end())
                {
                    it->second();
                }
            }
        }

        void update(float dt)
        {
            mPhysicsSystem.update(dt);
            mCollisionSystem.update(dt);
        }

        SDL_Rect getBoundingBox(Entity entity)
        {
            return mpBoundingBoxComponent->getBoundingBox(entity);
        }

        SDL_Rect getIntersection(Entity a, Entity b)
        {
            SDL_Rect aRect = getBoundingBox(a);
            SDL_Rect bRect = getBoundingBox(b);
            return te::getIntersection(aRect, bRect);
        }

        void draw()
        {
            mRenderSystem.draw();
        }

        void forEachEntity(const std::function<void(const Entity&)>& func)
        {
            std::for_each(mEntities.begin(), mEntities.end(), func);
        }

    private:
        std::vector<Entity> mEntities;

        TransformPtr mpTransformComponent;
        PhysicsPtr mpPhysicsComponent;
        BoundingBoxPtr mpBoundingBoxComponent;
        SimpleRenderPtr mpRenderComponent;

        EntityManager mEntityManager;

        PhysicsSystem mPhysicsSystem;
        CollisionSystem mCollisionSystem;

        TransformationData mTransformationData;

        RenderSystem mRenderSystem;

        std::map<char, std::function<void(void)>> mKeyPressTable;
        std::map<char, std::function<void(void)>> mKeyReleaseTable;

        CollisionHandler mCollisionHandler;

        FontHandle mFontCount;
        std::map<FontHandle, FontPtr> mFontMap;

        unsigned long mChannel;

        //std::map<Entity, GLuint> mVBOMap;
        //std::map<Entity, GLuint> mIBOMap;
    };
}

namespace te
{
    class TiledMap
    {
    public:
        TiledMap(const std::string& path, const std::string& filename)
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
            int status = luaL_dofile(L.get(), std::string{ path + "/" + filename }.c_str());

            if (status) { throw std::runtime_error("Could not load script."); }

            luabridge::LuaRef tiledData(luabridge::getGlobal(L.get(), "tiledData"));

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

        TiledMap(TiledMap&& tm)
            : mShaderProgram(tm.mShaderProgram)
            , mTilesets(std::move(tm.mTilesets))
            , mBuffers(std::move(tm.mBuffers))
        {
            tm.mShaderProgram = 0;
        }

        TiledMap& operator=(TiledMap&& tm)
        {
            destroy();

            mShaderProgram = tm.mShaderProgram;
            mTilesets = std::move(tm.mTilesets);
            mBuffers = std::move(tm.mBuffers);

            tm.mShaderProgram = 0;

            return *this;
        }

        ~TiledMap()
        {
            destroy();
        }

        void draw() const
        {
            glUseProgram(mShaderProgram);

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

        struct Buffers
        {
            GLuint vao;
            GLuint vbo;
            GLuint ebo;
        };

        GLuint mShaderProgram;
        std::vector<Tileset> mTilesets;
        std::vector<Buffers> mBuffers;

        void destroy()
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
    };
}

void adjustViewport(int screen_width, int screen_height)
{
    const float VIRTUAL_WIDTH = 16.0f;
    const float VIRTUAL_HEIGHT = 9.0f;

    float targetAspectRatio = VIRTUAL_WIDTH / VIRTUAL_HEIGHT;
    int width = screen_width;
    int height = (int)(width / targetAspectRatio + 0.5f);

    if (height > screen_height)
    {
        height = screen_height;
        width = (int)(height * targetAspectRatio + 0.5);
    }

    int vp_x = (screen_width / 2) - (width / 2);
    int vp_y = (screen_height / 2) - (height / 2);

    glViewport(vp_x, vp_y, width, height);
}

int main(int argc, char** argv)
{
    try
    {
        te::Initialization init;

        const int WIDTH = 640;
        const int HEIGHT = 480;


        te::WindowPtr pGLWindow = te::createWindowOpenGL("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        adjustViewport(WIDTH, HEIGHT);

        glClearColor(0.f, 0.f, 0.f, 1.f);

        GLuint programID = loadProgram("vertex.glvs", "fragment.glfs");
        glUseProgram(programID);

        GLint projectionMatrixLocation = glGetUniformLocation(programID, "te_ProjectionMatrix");
        if (projectionMatrixLocation == -1) { throw std::runtime_error("te_ProjectionMatrix: not a valid program variable."); }
        glm::mat4 projectionMatrix = glm::ortho<GLfloat>(0.0, 100, 100, 0.0, 1.0, -1.0);
        glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

        GLint modelViewMatrixLocation = glGetUniformLocation(programID, "te_ModelViewMatrix");
        if (modelViewMatrixLocation == -1) { throw std::runtime_error("te_ModelViewMatrix: not a valid program variable."); }
        glm::mat4 modelViewMatrix;
        glUniformMatrix4fv(modelViewMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

        GLint vertex2DPositionLocation = glGetAttribLocation(programID, "te_VertexPos2D");
        if (vertex2DPositionLocation == -1) { throw std::runtime_error("te_Vertex2DPos2D: not a valid program variable."); }

        LuaGameState state({projectionMatrix, modelViewMatrix, projectionMatrixLocation, modelViewMatrixLocation, vertex2DPositionLocation});

        te::TiledMap myMap("tiled", "sample.lua");
        te::TiledMap yourMap = std::move(myMap);
        myMap = std::move(yourMap);

        // State initialization

        Entity ball = state.createEntity({ 50, 50 }, { 50, 0 });
        state.setSprite(ball, { 5, 5 });
        state.setBoundingBox(ball, { 5, 5 });

        //Entity follower = state.createEntity({ 0, 30 }, { 0, 0 });
        //state.setSprite(follower, { 25, 25 });
        //state.setParent(follower, ball);
        //state.scale(ball, { 0.5, 2, 0 });

        Entity leftPaddle = state.createEntity({ 1, 50 });
        state.setSprite(leftPaddle, { 2, 20 });
        state.setBoundingBox(leftPaddle, { 2, 20 });

        Entity rightPaddle = state.createEntity({ 99, 50 });
        state.setSprite(rightPaddle, { 2, 20 });
        state.setBoundingBox(rightPaddle, { 2, 20 });

        auto handlePaddleCollision = [&state](Entity ball, Entity paddle, float dt)
        {
            glm::vec2 ballPos = state.getPosition(ball);
            glm::vec2 paddlePos = state.getPosition(paddle);
            glm::vec2 paddleToBall = ballPos - paddlePos;
            glm::vec2 ballVel = state.getVelocity(ball);
            glm::vec2 velocity = glm::length(ballVel) * glm::normalize(paddleToBall);
            state.setVelocity(ball, velocity);
            state.playSound();
        };
        state.handleCollision(ball, leftPaddle, handlePaddleCollision);
        state.handleCollision(ball, rightPaddle, handlePaddleCollision);

        state.registerKeyPress('w', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, -100)); });
        state.registerKeyPress('s', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 100)); });
        state.registerKeyPress('p', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, -100)); });
        state.registerKeyPress('l', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, 100)); });

        state.registerKeyRelease('w', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('s', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('p', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('l', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, 0)); });

        Entity topWall = state.createEntity({ 50, -1 });
        state.setBoundingBox(topWall, { 100, 2 });
        Entity bottomWall = state.createEntity({ 50, 101 });
        state.setBoundingBox(bottomWall, { 100, 2 });

        auto handleWallCollision = [&state](Entity ball, Entity wall, float dt)
        {
            glm::vec2 velocity = state.getVelocity(ball);
            SDL_Rect intersection = state.getIntersection(ball, wall);
            if (intersection.w > intersection.h)
            {
                velocity.y = -velocity.y;
                state.setVelocity(ball, velocity);
            }
            else
            {
                velocity.x = -velocity.x;
                state.setVelocity(ball, velocity);
            }
        };
        state.handleCollision(ball, topWall, handleWallCollision);
        state.handleCollision(ball, bottomWall, handleWallCollision);

        // End state initialization

        SDL_Event e;
        bool running = true;

        Uint32 FPS = 60;
        Uint32 TIME_PER_FRAME = 1000 / FPS;

        Uint64 t0 = SDL_GetPerformanceCounter();

        while (running)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    running = false;
                }
                else if (e.type == SDL_WINDOWEVENT)
                {
                    if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                    {
                        adjustViewport(e.window.data1, e.window.data2);
                    }
                }
                state.processInput(e);
            }

            Uint64 now = SDL_GetPerformanceCounter();
            float dt = (float)(now - t0) / SDL_GetPerformanceFrequency();

            state.update(dt);

            glClear(GL_COLOR_BUFFER_BIT);

            myMap.draw();
            glUseProgram(programID);
            state.draw();

            SDL_GL_SwapWindow(pGLWindow.get());
            t0 = now;
        }
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
