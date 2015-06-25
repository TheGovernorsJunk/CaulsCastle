#include <SDL.h>
#include <iostream>
#include <math.h>
#include <map>
#include <functional>
#include <lua.hpp>
#include "types.h"
#include "wrappers.h"
#include "auxiliary.h"
#include "entity.h"
#include "commands.h"
#include "player.h"

using namespace te;

int main(int argc, char** argv)
{
    std::shared_ptr<lua_State> L(luaL_newstate(), [](lua_State* L){ lua_close(L); });



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
