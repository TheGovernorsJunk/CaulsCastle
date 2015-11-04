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
#include <queue>
#include "types.h"
#include "wrappers.h"
#include "auxiliary.h"
#include "player.h"
#include "entity_manager.h"
#include "transform_component.h"
#include "physics_component.h"
#include "physics_system.h"
#include "platformer_physics_system.h"
#include "bounding_box_component.h"
#include "collision_system.h"
#include "simple_render_component.h"
#include "render_system.h"
#include "texture.h"
#include "shader.h"
#include "tiled_map.h"
#include "game_state.h"
#include "tmx.h"
#include "texture_manager.h"
#include "mesh_manager.h"
#include "animation_component.h"
#include "animation_factory.h"

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

    class CollisionHandler : public Observer<CollisionEvent>, public Observer<MapCollisionEvent>
    {
    public:
        typedef std::pair<Entity, Entity> EntityPair;
        typedef std::pair<Entity, TiledMap*> EntityMapPair;

        std::map<EntityPair, std::function<void(Entity, Entity, float)>> map;
        std::map<EntityMapPair, std::function<void(Entity, TiledMap&, float)>> tiledCollisionMap;

        virtual void onNotify(const CollisionEvent& evt)
        {
            EntityPair pair = { evt.a, evt.b };
            auto it = map.find(pair);
            if (it != map.end())
            {
                it->second(evt.a, evt.b, evt.dt);
            }
        }

        virtual void onNotify(const MapCollisionEvent& evt)
        {
            EntityMapPair pair = { evt.entity, &evt.map };
            auto it = tiledCollisionMap.find(pair);
            if (it != tiledCollisionMap.end())
            {
                it->second(evt.entity, evt.map, evt.dt);
            }
        }
    };

    class PauseState : public GameState
    {
    public:
        bool processInput(const SDL_Event& evt)
        {
            if (evt.type == SDL_KEYDOWN)
            {
                if (evt.key.keysym.sym == SDLK_SPACE)
                {
                    queuePop();
                }
                else if (evt.key.keysym.sym == SDLK_q)
                {
                    queueClear();
                }
            }
            return false;
        }

        bool update(float dt)
        {
            return false;
        }

        void draw()
        {
            //glClearColor(0.f, 1.f, 0.f, 0.5f);
            //glClear(GL_COLOR_BUFFER_BIT);
        }
    };

    class LuaGameState : public GameState
    {
    public:
        LuaGameState(const glm::mat4& projection, const std::string& filename = "init.lua")
            : GameState()
            , mpShader(new Shader(glm::ortho<GLfloat>(0, 16 * 16, 9 * 16, 0, -100, 100), glm::mat4()))
            , mpTextureManager(new TextureManager())
            , mpTMX(new TMX("tiled", "sample_map.lua"))
            , mpMap(new TiledMap(*mpTMX, mpShader, mpTextureManager.get()))
            , mCollisionHandler(new CollisionHandler())
            , mpTransformComponent(new TransformComponent())
            , mpPhysicsComponent(new PhysicsComponent())
            , mpBoundingBoxComponent(new BoundingBoxComponent(mpTransformComponent))
            , mpRenderComponent(new SimpleRenderComponent(projection))
            , mpAnimationComponent(new AnimationComponent())
            , mEntityManager({mpTransformComponent, mpPhysicsComponent, mpBoundingBoxComponent, mpRenderComponent})
            , mPhysicsSystem(mpPhysicsComponent, mpTransformComponent, mpBoundingBoxComponent, mpMap, 4)
            , mCollisionSystem(mpBoundingBoxComponent, {mCollisionHandler})
            , mMapCollisionSystem(mpBoundingBoxComponent, mpMap, {mCollisionHandler})
            , mRenderSystem(mpShader, mpRenderComponent, mpAnimationComponent, mpTransformComponent)
            , mKeyPressTable()
            , mKeyReleaseTable()
            , mFontCount(0)
            , mFontMap()
            , mChannel(0)
            , mView()
        {
            if (!(mChannel = BASS_StreamCreateFile(false, "sounds/arcade-beep-1.mp3", 0, 0, 0)))
            {
                throw std::runtime_error("Could not load sound.");
            }
            std::shared_ptr<MeshManager> pMeshManager(new MeshManager{ mpTMX, mpTextureManager });
            Entity e = createEntity({ 0,0 });
            setPosition(e, glm::vec3(16 * 3, 16 * 3, 10));
            //ac.setAnimations(e, *pTMX, pTMX->layers[3].objects[0], *pMeshManager);
            std::shared_ptr<AnimationFactory> pAnimationFactory(new AnimationFactory{ mpTMX, pMeshManager });

            std::shared_ptr<const Animation> pAnimation(new Animation(pAnimationFactory->create({
                {"animation", "walking"},
                {"character", "amygdala"}
            }, true)));
            std::shared_ptr<const Animation> pJesterAnimation(new Animation(pAnimationFactory->create({
                {"animation", "walking"},
                {"type", "jester"}
            })));
            // Should be frozen using default tile mesh
            std::shared_ptr<const Animation> pMonsterStill(new Animation(pAnimationFactory->create({
                {"type", "monster"}
            })));

            mpAnimationComponent->setAnimations(e, {
                {1, pAnimation},
                {2, pJesterAnimation},
                {3, pMonsterStill}
            }, 1);

            registerKeyPress('1', [=] {
                mpAnimationComponent->setAnimation(e, 1);
            });
            registerKeyPress('2', [=] {
                mpAnimationComponent->setAnimation(e, 2);
            });
            registerKeyPress('3', [=] {
                mpAnimationComponent->setAnimation(e, 3);
            });

            loadObjects(mpTMX, pMeshManager, mEntityManager, *mpTransformComponent, *mpAnimationComponent);
        }

        typedef unsigned int EntityHandle;
        typedef std::pair<Entity, Entity> EntityPair;
        typedef luabridge::LuaRef LuaVector;
        typedef luabridge::LuaRef LuaFunction;
        typedef unsigned int FontHandle;
        typedef unsigned int TextHandle;

        void pause()
        {
            queuePush(std::shared_ptr<PauseState>(new PauseState()));
        }

        void playSound()
        {
            BASS_ChannelPlay(mChannel, true);
        }

        Entity createEntity(glm::vec2 position, glm::vec2 velocity = glm::vec2(0, 0))
        {
            Entity entity = mEntityManager.create();
            mpTransformComponent->setLocalTransform(
                entity,
                glm::translate(glm::vec3(position.x, position.y, 0.f)));
            if (velocity != glm::vec2(0, 0))
            {
                mpPhysicsComponent->setPhysics(entity, velocity);
            }
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

        void setPosition(Entity entity, glm::vec3 position)
        {
            if (!exists(entity)) return;

            mpTransformComponent->setLocalTransform(
                entity,
                glm::translate(glm::mat4(), glm::vec3(position.x, position.y, position.z)));
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
            auto it = mCollisionHandler->map.find(key);
            if (it == mCollisionHandler->map.end())
            {
                mCollisionHandler->map.insert(std::make_pair(
                    key,
                    handler));
            }
            else
            {
                it->second = handler;
            }
        }

        void handleMapCollision(Entity e, std::function<void(Entity, TiledMap&, float)> handler)
        {
            if (!exists(e)) return;

            auto key = std::make_pair(e, mpMap.get());
            auto it = mCollisionHandler->tiledCollisionMap.find(key);
            if (it == mCollisionHandler->tiledCollisionMap.end())
            {
                mCollisionHandler->tiledCollisionMap.insert(std::make_pair(
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

        bool processInput(const SDL_Event& evt)
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
            else if (evt.type == SDL_WINDOWEVENT)
            {
                if (evt.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    adjustViewport(evt.window.data1, evt.window.data2, 16.f / 9.f, glViewport);
                }
            }
            return false;
        }

        bool update(float dt)
        {
            mPhysicsSystem.update(dt);
            mCollisionSystem.update(dt);
            mRenderSystem.update(dt);
            //mMapCollisionSystem.update(dt);

            return false;
        }

        BoundingBox getBoundingBox(Entity entity)
        {
            return mpBoundingBoxComponent->getBoundingBox(entity);
        }

        BoundingBox getIntersection(Entity a, Entity b)
        {
            BoundingBox aRect = getBoundingBox(a);
            BoundingBox bRect = getBoundingBox(b);
            return te::getIntersection(aRect, bRect);
        }

        // HACKY! Only for test driver project
        void setView(const glm::mat4& view)
        {
            mView = view;
        }

        glm::mat4 getView() const
        {
            return mView;
        }

        void draw()
        {
            //mMap.draw(mView);
            mpMap->draw(mView);
            mRenderSystem.draw(mView);
        }

    private:
        std::shared_ptr<Shader> mpShader;
        std::shared_ptr<TextureManager> mpTextureManager;

        std::shared_ptr<const TMX> mpTMX;
        std::shared_ptr<TiledMap> mpMap;
        std::shared_ptr<CollisionHandler> mCollisionHandler;

        TransformPtr mpTransformComponent;
        PhysicsPtr mpPhysicsComponent;
        BoundingBoxPtr mpBoundingBoxComponent;
        SimpleRenderPtr mpRenderComponent;
        AnimationPtr mpAnimationComponent;

        EntityManager mEntityManager;

        PlatformerPhysicsSystem mPhysicsSystem;
        CollisionSystem mCollisionSystem;
        MapCollisionSystem mMapCollisionSystem;

        RenderSystem mRenderSystem;

        std::map<char, std::function<void(void)>> mKeyPressTable;
        std::map<char, std::function<void(void)>> mKeyReleaseTable;

        FontHandle mFontCount;
        std::map<FontHandle, FontPtr> mFontMap;

        unsigned long mChannel;

        //std::map<Entity, GLuint> mVBOMap;
        //std::map<Entity, GLuint> mIBOMap;
        glm::mat4 mView;
    };

    class TopdownState : public GameState
    {
    public:
        TopdownState()
            : GameState()
        {
            //Texture spritesheet("tiled/roguelike-indoor-pack/Spritesheet/roguelikeindoor_transparent.png");
        }

    private:
        bool processInput(const SDL_Event& e)
        {
            return false;
        }
        bool update(float dt)
        {
            return false;
        }
        void draw() {}
    };
}

int main(int argc, char** argv)
{
    try
    {
        te::Initialization init;

        const int WIDTH = 1024;
        const int HEIGHT = 576;


        te::WindowPtr pGLWindow = te::createWindowOpenGL("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        adjustViewport(WIDTH, HEIGHT, 16.f/9.f, glViewport);

        glClearColor(0.f, 0.f, 0.f, 1.f);


        glm::mat4 projection = glm::ortho<GLfloat>(0, 16, 9, 0, -1, 100);

        std::shared_ptr<LuaGameState> pState(new LuaGameState(projection));
        std::shared_ptr<TopdownState> pTopdown(new TopdownState());
        StateStack stateStack(pState);
        LuaGameState& state = *pState;

        // State initialization

        Entity ball = state.createEntity({ 8, 4.5 }, { 0, -1 });
        state.setSprite(ball, { 1, 1 });
        state.setBoundingBox(ball, { 1, 1 });

        Entity leftPaddle = state.createEntity({ 0.5, 4.5 });
        state.setSprite(leftPaddle, { 1, 3 });
        state.setBoundingBox(leftPaddle, { 1, 3 });

        Entity rightPaddle = state.createEntity({ 15.5, 4.5 });
        state.setSprite(rightPaddle, { 1, 3 });
        state.setBoundingBox(rightPaddle, { 1, 3 });

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

        auto handleGroundCollision = [&state](Entity paddle, TiledMap& map, float dt)
        {
            glm::vec2 paddleVel = state.getVelocity(paddle);
            if (paddleVel.y > 0)
            {
                paddleVel.y = 0;
                state.setVelocity(paddle, paddleVel);
            }
        };
        state.handleMapCollision(leftPaddle, handleGroundCollision);

        state.registerKeyPress('w', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, -8)); });
        state.registerKeyPress('s', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 8)); });
        state.registerKeyPress('a', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(-8, 0)); });
        state.registerKeyPress('d', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(8, 0)); });
        state.registerKeyPress('p', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, -8)); });
        state.registerKeyPress('l', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, 8)); });

        state.registerKeyRelease('w', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('s', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('a', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('d', [leftPaddle, &state]() { state.setVelocity(leftPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('p', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, 0)); });
        state.registerKeyRelease('l', [rightPaddle, &state]() { state.setVelocity(rightPaddle, glm::vec2(0, 0)); });

        state.registerKeyPress(' ', [&state]() { state.pause(); });
        state.registerKeyPress((char)SDLK_UP, [&state]() { state.setView(glm::translate(state.getView(), glm::vec3(0.f, 16.f, 0.f))); });
        state.registerKeyPress((char)SDLK_DOWN, [&state]() { state.setView(glm::translate(state.getView(), glm::vec3(0.f, -16.f, 0.f))); });
        state.registerKeyPress((char)SDLK_RIGHT, [&state]() { state.setView(glm::translate(state.getView(), glm::vec3(-16.f, 0.f, 0.f))); });
        state.registerKeyPress((char)SDLK_LEFT, [&state]() { state.setView(glm::translate(state.getView(), glm::vec3(16.f, 0.f, 0.f))); });

        Entity topWall = state.createEntity({ 50, -1 });
        state.setBoundingBox(topWall, { 100, 2 });
        Entity bottomWall = state.createEntity({ 8, 10 });
        state.setBoundingBox(bottomWall, { 100, 2 });

        auto handleWallCollision = [&state](Entity ball, Entity wall, float dt)
        {
            glm::vec2 velocity = state.getVelocity(ball);
            BoundingBox intersection = state.getIntersection(ball, wall);
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

        executeStack(stateStack, *pGLWindow);
    }
    catch (std::exception e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
