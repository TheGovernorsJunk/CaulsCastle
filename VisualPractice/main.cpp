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
        {
            lua_State* pL = mpL.get();
            luaL_openlibs(pL);

            luabridge::getGlobalNamespace(pL)
                .beginClass<LuaGameState>("GameState")
                .addFunction("createEntity", &LuaGameState::createEntity)
                .addFunction("destroyEntity", &LuaGameState::destroyEntity)
                .endClass();
            luabridge::push(pL, this);
            lua_setglobal(pL, "game");

            luaL_dofile(pL, filename.c_str());
            luabridge::getGlobal(pL, "main")();
        }

        typedef unsigned int EntityHandle;

        EntityHandle createEntity(float x, float y, float dx, float dy)
        {
            EntityHandle handle = mHandleCount++;
            mEntities.push_back(handle);
            mPositionMap.insert(std::make_pair(handle, Vector2f(x, y)));
            mVelocityMap.insert(std::make_pair(handle, Vector2f(dx, dy)));
            return handle;
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
        }

        void update(float dt)
        {
            forEachEntity([&](const EntityHandle& handle)
            {
                Vector2f& velocity = mVelocityMap.find(handle)->second;
                Vector2f& position = mPositionMap.find(handle)->second;
                position.x += velocity.x * dt;
                position.y += velocity.y * dt;
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
    };
}

int main(int argc, char** argv)
{
    LuaGameState state;

    const int WIDTH = 640;
    const int HEIGHT = 480;

    te::Initialization init;
    te::WindowPtr pWindow = te::wrapWindow(
        SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN)
    );
    te::RendererPtr pRenderer = te::createRenderer(pWindow);
    SDL_SetRenderDrawColor(pRenderer.get(), 0x00, 0x00, 0x00, 0xFF);

    SDL_Rect ballRect = { 0, 0, 25, 25 };
    float x = 0;
    float ballSpeed = 50;

    Rectangle dot(300.f, 110.f, 25, 25, -200.f, 0.f);
    std::shared_ptr<Rectangle> pPaddle1(new Rectangle(600.f, 30.f, 25, 200, 0.f, 0.f));
    std::shared_ptr<Rectangle> pPaddle2(new Rectangle(50.f, 30.f, 25, 200, 0.f, 0.f));
    Rectangle topWall(0.f, 0.f, 640, 10);
    Rectangle bottomWall(0.f, 470.f, 640, 10);

    KeyMap keys = createPaddleKeyMap();

    Player player1(pPaddle1, 1);
    Player player2(pPaddle2, 2);

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
            player1.issueCommand(e.key.keysym.sym, e.type)();
            player2.issueCommand(e.key.keysym.sym, e.type)();
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - t0) / SDL_GetPerformanceFrequency();

        dot.update(dt);
        pPaddle1->update(dt);
        pPaddle2->update(dt);
        topWall.update(dt);
        bottomWall.update(dt);

        handlePaddleCollision(dot, *pPaddle1.get(), dt);
        handlePaddleCollision(dot, *pPaddle2.get(), dt);
        handleWallCollision(dot, topWall, dt);
        handleWallCollision(dot, bottomWall, dt);

        SDL_SetRenderDrawColor(pRenderer.get(), 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(pRenderer.get());

        dot.draw(pRenderer);
        pPaddle1->draw(pRenderer);
        pPaddle2->draw(pRenderer);
        topWall.draw(pRenderer);
        bottomWall.draw(pRenderer);

        SDL_RenderPresent(pRenderer.get());
        t0 = now;
    }

    return 0;
}
