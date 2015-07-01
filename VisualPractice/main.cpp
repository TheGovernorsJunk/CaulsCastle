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
            , mPendingDestroys()
            , mKeyPressTable(luabridge::newTable(mpL.get()))
            , mKeyReleaseTable(luabridge::newTable(mpL.get()))
            , mCollisionHandlerMap()
        {
            lua_State* pL = mpL.get();
            luaL_openlibs(pL);

            luaL_dofile(pL, "types.lua");

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
                .addFunction("destroyEntity", &LuaGameState::addPendingDestroy)
                .addFunction("registerKeyPressTable", &LuaGameState::registerKeyPressTable)
                .addFunction("registerKeyReleaseTable", &LuaGameState::registerKeyReleaseTable)
                .endClass()
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
        typedef luabridge::LuaRef LuaVector;
        typedef luabridge::LuaRef LuaFunction;

        template<typename T>
        LuaVector luaVector(const Vector2<T>& v)
        {
            luabridge::LuaRef LuaVector = luabridge::getGlobal(mpL.get(), "Vector");
            return LuaVector["new"](LuaVector, v.x, v.y);
        }

        template<typename T>
        Vector2<T> cppVector(const LuaVector& v)
        {
            return Vector2<T>(v["x"], v["y"]);
        }

        EntityHandle createEntity(LuaVector position, LuaVector velocity)
        {
            EntityHandle handle = mHandleCount++;
            mEntities.push_back(handle);
            mPositionMap.insert(std::make_pair(handle, cppVector<float>(position)));
            mVelocityMap.insert(std::make_pair(handle, cppVector<float>(velocity)));
            mBoundingBoxMap.insert(std::make_pair(handle, Vector2i(0, 0)));
            return handle;
        }

        void setPosition(EntityHandle handle, LuaVector position)
        {
            if (!exists(handle)) return;

            mPositionMap[handle] = cppVector<float>(position);
        }

        LuaVector getPosition(EntityHandle handle)
        {
            if (!exists(handle)) return luaVector(Vector2f(0.f, 0.f));

            return luaVector(mPositionMap[handle]);
        }

        void setVelocity(EntityHandle handle, LuaVector velocity)
        {
            if (!exists(handle)) return;

            mVelocityMap[handle] = cppVector<float>(velocity);
        }

        LuaVector getVelocity(EntityHandle handle)
        {
            if (!exists(handle)) return luaVector(Vector2f(0.f, 0.f));

            return luaVector(mVelocityMap[handle]);
        }

        void setBoundingBox(EntityHandle handle, LuaVector dimensions)
        {
            if (!exists(handle)) return;

            mBoundingBoxMap[handle] = cppVector<int>(dimensions);
        }

        void setSprite(EntityHandle handle, LuaVector dimensions)
        {
            if (!exists(handle)) return;

            insertOrAssign(mDimensionMap, std::make_pair(
                handle, cppVector<int>(dimensions)));
        }

        void handleCollision(EntityHandle e1, EntityHandle e2, LuaFunction handler)
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

        void addPendingDestroy(EntityHandle handle)
        {
            mPendingDestroys.push_back(handle);
        }

        void destroyEntity(EntityHandle handle)
        {
            if (!exists(handle)) return;

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
            mEntities.erase(
                std::remove(mEntities.begin(), mEntities.end(), handle),
                mEntities.end());
        }

        void destroyEntities()
        {
            std::for_each(
                mPendingDestroys.begin(),
                mPendingDestroys.end(),
                [&](const EntityHandle& handle)
            {
                destroyEntity(handle);
            });
            mPendingDestroys.clear();
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
            destroyEntities();
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

        std::vector<EntityHandle> mPendingDestroys;

        luabridge::LuaRef mKeyPressTable;
        luabridge::LuaRef mKeyReleaseTable;

        std::map<EntityPair, LuaFunction> mCollisionHandlerMap;
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
