#include <SDL.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>
#include <lua.hpp>
#include <LuaBridge.h>
#include "types.h"
#include "wrappers.h"
#include "auxiliary.h"
#include "entity.h"
#include "commands.h"
#include "player.h"

using namespace te;

namespace te
{
    class LuaGameState
    {
    public:
        LuaGameState(const std::string& filename = "init.lua")
            : mpL(luaL_newstate(), [](lua_State* L){ lua_close(L); })
            , mHandleCount(0)
            , mEntities()
            , mPositionMap()
            , mVelocityMap()
            , mBoundingBoxMap()
            , mDimensionMap()
            , mKeyPressTable(luabridge::newTable(mpL.get()))
            , mKeyReleaseTable(luabridge::newTable(mpL.get()))
            , mCollisionHandlerMap()
        {
            lua_State* pL = mpL.get();
            luaL_openlibs(pL);

            luabridge::getGlobalNamespace(pL)
                .beginClass<LuaGameState>("GameState")
                .addFunction("createEntity", &LuaGameState::createEntity)
                .addFunction("setPosition", &LuaGameState::setPosition)
                .addFunction("getPosition", &LuaGameState::getPosition)
                .addFunction("setVelocity", &LuaGameState::setVelocity)
                .addFunction("getVelocity", &LuaGameState::getVelocity)
                .addFunction("setBoundingBox", &LuaGameState::setBoundingBox)
                .addFunction("getBoundingBox", &LuaGameState::getBoundingBox)
                .addFunction("getIntersection", &LuaGameState::getIntersection)
                .addFunction("setSprite", &LuaGameState::setSprite)
                .addFunction("handleCollision", &LuaGameState::handleCollision)
                .addFunction("destroyEntity", &LuaGameState::destroyEntity)
                .addFunction("registerKeyPressTable", &LuaGameState::registerKeyPressTable)
                .addFunction("registerKeyReleaseTable", &LuaGameState::registerKeyReleaseTable)
                .endClass()
                .beginClass<Vector2f>("Vector2")
                .addConstructor<void(*)(void)>()
                .addConstructor<void(*)(float, float)>()
                .addData("x", &te::Vector2f::x)
                .addData("y", &te::Vector2f::y)
                .endClass()
                .addFunction<te::Vector2f(*)(te::Vector2f, te::Vector2f)>("addV", &te::operator+)
                .addFunction<te::Vector2f(*)(te::Vector2f, te::Vector2f)>("subtractV", &te::operator-)
                .addFunction<te::Vector2f(*)(float, te::Vector2f)>("multiplyV", &te::operator*)
                .addFunction<te::Vector2f(*)(te::Vector2f, float)>("divideV", &te::operator/)
                .addFunction<float(*)(te::Vector2f)>("lengthV", &te::length)
                .addFunction<te::Vector2f(*)(te::Vector2f)>("normalizeV", &te::normalize)
                .beginClass<SDL_Rect>("Rect")
                .addData("h", &SDL_Rect::h)
                .addData("w", &SDL_Rect::w)
                .addData("x", &SDL_Rect::x)
                .addData("y", &SDL_Rect::y)
                .endClass();
            luabridge::push(pL, this);
            lua_setglobal(pL, "game");

            luaL_dofile(pL, filename.c_str());
            luabridge::getGlobal(pL, "main")();
        }

        typedef unsigned int EntityHandle;
        typedef std::pair<EntityHandle, EntityHandle> EntityPair;

        EntityHandle createEntity(const Vector2f& position, const Vector2f& velocity)
        {
            EntityHandle handle = mHandleCount++;
            mEntities.push_back(handle);
            mPositionMap.insert(std::make_pair(handle, position));
            mVelocityMap.insert(std::make_pair(handle, velocity));
            mBoundingBoxMap.insert(std::make_pair(handle, Vector2i(0, 0)));
            return handle;
        }

        void setPosition(EntityHandle handle, const Vector2f& position)
        {
            if (!exists(handle)) return;

            mPositionMap[handle] = position;
        }

        Vector2f getPosition(EntityHandle handle)
        {
            if (!exists(handle)) return Vector2f(0.f, 0.f);

            return mPositionMap[handle];
        }

        void setVelocity(EntityHandle handle, const Vector2f& velocity)
        {
            if (!exists(handle)) return;

            mVelocityMap[handle] = velocity;
        }

        Vector2f getVelocity(EntityHandle handle)
        {
            if (!exists(handle)) return Vector2f(0.f, 0.f);

            return mVelocityMap[handle];
        }

        void setBoundingBox(EntityHandle handle, const Vector2f& dimensions)
        {
            if (!exists(handle)) return;

            mBoundingBoxMap[handle] = convertVector2<int>(dimensions);
        }

        void setSprite(EntityHandle handle, const Vector2f& dimensions)
        {
            if (!exists(handle)) return;

            insertOrAssign(mDimensionMap, std::make_pair(
                handle, convertVector2<int>(dimensions)));
        }

        void handleCollision(EntityHandle e1, EntityHandle e2, luabridge::LuaRef handler)
        {
            if (!exists(e1) || !exists(e2)) return;

            auto key = std::make_pair(e1, e2);
            auto it = mCollisionHandlerMap.find(key);
            if (it == mCollisionHandlerMap.end())
            {
                mCollisionHandlerMap.insert(std::make_pair(
                    key,
                    handler));
            }
            else
            {
                it->second = handler;
            }
        }

        void registerKeyPressTable(luabridge::LuaRef table)
        {
            mKeyPressTable = table;
        }

        void registerKeyReleaseTable(luabridge::LuaRef table)
        {
            mKeyReleaseTable = table;
        }

        bool exists(EntityHandle handle)
        {
            auto it = std::find(std::begin(mEntities), std::end(mEntities), handle);
            return it != std::end(mEntities);
        }

        void destroyEntity(EntityHandle handle)
        {
            mEntities.erase(
                std::remove(mEntities.begin(), mEntities.end(), handle),
                mEntities.end());
            auto positionIt = mPositionMap.find(handle);
            if (positionIt != mPositionMap.end())
            {
                mPositionMap.erase(positionIt);
            }
            auto velocityIt = mVelocityMap.find(handle);
            if (velocityIt != mVelocityMap.end())
            {
                mVelocityMap.erase(velocityIt);
            }
            auto boundingBoxIt = mBoundingBoxMap.find(handle);
            if (boundingBoxIt != mBoundingBoxMap.end())
            {
                mBoundingBoxMap.erase(boundingBoxIt);
            }
            auto dimensionIt = mDimensionMap.find(handle);
            if (dimensionIt != mDimensionMap.end())
            {
                mDimensionMap.erase(dimensionIt);
            }
        }

        void processInput(const SDL_Event& evt)
        {
            if (evt.type == SDL_KEYDOWN)
            {
                if (mKeyPressTable[evt.key.keysym.sym].isFunction())
                {
                    mKeyPressTable[evt.key.keysym.sym]();
                }
            }
            else if (evt.type == SDL_KEYUP)
            {
                if (mKeyReleaseTable[evt.key.keysym.sym].isFunction())
                {
                    mKeyReleaseTable[evt.key.keysym.sym]();
                }
            }
        }

        void update(float dt)
        {
            forEachEntity([&](const EntityHandle& handle)
            {
                mPositionMap[handle] += dt * mVelocityMap[handle];
            });
            std::for_each(
                mCollisionHandlerMap.begin(),
                mCollisionHandlerMap.end(),
                [&](std::pair<const EntityPair, luabridge::LuaRef> kv)
            {
                if (checkCollision(
                    getBoundingBox(kv.first.first),
                    getBoundingBox(kv.first.second)))
                {
                    kv.second(kv.first.first, kv.first.second, dt);
                }
            });
        }

        SDL_Rect getBoundingBox(EntityHandle handle)
        {
            Vector2f position = mPositionMap[handle];
            Vector2i boundingBox = mBoundingBoxMap[handle];
            return SDL_Rect{
                (int)position.x - (boundingBox.x / 2),
                (int)position.y - (boundingBox.y / 2),
                boundingBox.x,
                boundingBox.y
            };
        }

        SDL_Rect getIntersection(EntityHandle a, EntityHandle b)
        {
            SDL_Rect aRect = getBoundingBox(a);
            SDL_Rect bRect = getBoundingBox(b);
            return te::getIntersection(aRect, bRect);
        }

        void draw(RendererPtr pRenderer)
        {
            forEachEntity([&](const EntityHandle& handle)
            {
                auto positionIt = mPositionMap.find(handle);
                auto spriteIt = mDimensionMap.find(handle);
                if (spriteIt != mDimensionMap.end())
                {
                    Vector2f& pos = positionIt->second;
                    Vector2i& dim = spriteIt->second;
                    SDL_SetRenderDrawColor(pRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_Rect rect = {
                        (int)pos.x - (dim.x / 2),
                        (int)pos.y - (dim.y / 2),
                        dim.x,
                        dim.y
                    };
                    SDL_RenderFillRect(pRenderer.get(), &rect);
                }
            });
        }

        void forEachEntity(const std::function<void(const EntityHandle&)>& func)
        {
            std::for_each(mEntities.begin(), mEntities.end(), func);
        }

    private:
        std::shared_ptr<lua_State> mpL;
        EntityHandle mHandleCount;

        std::vector<EntityHandle> mEntities;

        std::map<EntityHandle, Vector2f> mPositionMap;
        std::map<EntityHandle, Vector2f> mVelocityMap;
        std::map<EntityHandle, Vector2i> mBoundingBoxMap;
        std::map<EntityHandle, Vector2i> mDimensionMap;

        luabridge::LuaRef mKeyPressTable;
        luabridge::LuaRef mKeyReleaseTable;
        std::map<EntityPair, luabridge::LuaRef> mCollisionHandlerMap;
    };
}

int main(int argc, char** argv)
{
    te::Initialization init;

    LuaGameState state;

    const int WIDTH = 640;
    const int HEIGHT = 480;

    te::WindowPtr pWindow = te::wrapWindow(
        SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN)
    );
    te::RendererPtr pRenderer = te::createRenderer(pWindow);

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
            state.processInput(e);
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - t0) / SDL_GetPerformanceFrequency();

        state.update(dt);

        SDL_SetRenderDrawColor(pRenderer.get(), 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(pRenderer.get());

        state.draw(pRenderer);

        SDL_RenderPresent(pRenderer.get());
        t0 = now;
    }

    return 0;
}
