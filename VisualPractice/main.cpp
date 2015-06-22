#include <SDL.h>
#include <iostream>
#include <math.h>
#include "types.h"
#include "wrappers.h"
#include "auxiliary.h"
#include "entity.h"

using namespace te;

int main(int argc, char** argv)
{
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
    Rectangle paddle(600.f, 30.f, 25, 200, 0.f, 0.f);
    Rectangle paddle2(50.f, 30.f, 25, 200, 0.f, 0.f);
    Rectangle topWall(0.f, 0.f, 640, 10);
    Rectangle bottomWall(0.f, 470.f, 640, 10);

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
        }

        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - t0) / SDL_GetPerformanceFrequency();

        dot.update(dt);
        paddle.update(dt);
        paddle2.update(dt);
        topWall.update(dt);
        bottomWall.update(dt);

        handlePaddleCollision(dot, paddle, dt);
        handlePaddleCollision(dot, paddle2, dt);
        handleWallCollision(dot, topWall, dt);
        handleWallCollision(dot, bottomWall, dt);

        SDL_SetRenderDrawColor(pRenderer.get(), 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(pRenderer.get());

        dot.draw(pRenderer);
        paddle.draw(pRenderer);
        paddle2.draw(pRenderer);
        topWall.draw(pRenderer);
        bottomWall.draw(pRenderer);

        SDL_RenderPresent(pRenderer.get());
        t0 = now;
    }

    return 0;
}
