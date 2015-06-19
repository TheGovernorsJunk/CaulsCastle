#include <SDL.h>
#include <memory>
#include <iostream>
#include <math.h>
#include "wrappers.h"

struct Ball
{
    float x;
    float y;
    Uint32 w;
    Uint32 h;
    float dx;
    float dy;
};

struct Vector2f
{
    float x;
    float y;
};

struct Vector2i
{
    int x;
    int y;
};

class Entity
{
public:
    Entity(float x, float y, float dx = 0, float dy = 0)
        : mX(x), mY(y), mDx(dx), mDy(dy) {}
    void setVelocity(float dx, float dy)
    {
        mDx = dx;
        mDy = dy;
    }
    ~Entity() {}

    Vector2f getPosition() const
    {
        Vector2f pos = { mX, mY };
        return pos;
    }
    Vector2f getVelocity() const
    {
        Vector2f vel = { mDx, mDy };
        return vel;
    }
    void setPosition(Vector2f pos)
    {
        mX = pos.x;
        mY = pos.y;
    }
    void setVelocity(Vector2f vel)
    {
        mDx = vel.x;
        mDy = vel.y;
    }
    void update(float dt)
    {
        mX += mDx * dt;
        mY += mDy * dt;
    }
    virtual void draw(te::RendererPtr) const = 0;
    virtual SDL_Rect getBoundingBox() const = 0;
private:
    float mX;
    float mY;
    float mDx;
    float mDy;
};

class Rectangle : public Entity
{
public:
    Rectangle(float x, float y, int w, int h, float dx = 0.f, float dy = 0.f)
        : Entity(x, y, dx, dy), mW(w), mH(h) {}

    virtual SDL_Rect getBoundingBox() const
    {
        Vector2f pos = getPosition();
        SDL_Rect rect = { (int)pos.x, (int)pos.y, mW, mH };
        return rect;
    }
    virtual void draw(te::RendererPtr pRenderer) const
    {
        SDL_SetRenderDrawColor(pRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_Rect rect = getBoundingBox();
        SDL_RenderFillRect(pRenderer.get(), &rect);
    }
private:
    int mW;
    int mH;
};

bool checkCollision(const SDL_Rect& a, const SDL_Rect& b)
{
    int topA = a.y;
    int leftA = a.x;
    int bottomA = a.y + a.h;
    int rightA = a.x + a.w;

    int topB = b.y;
    int leftB = b.x;
    int bottomB = b.y + b.h;
    int rightB = b.x + b.w;

    bool isColliding = true;
    if (topA >= bottomB ||
        topB >= bottomA ||
        leftA >= rightB ||
        leftB >= rightA)
    {
        isColliding = false;
    }
    return isColliding;
}

bool checkCollision(const Rectangle& a, const Rectangle& b)
{
    return checkCollision(a.getBoundingBox(), b.getBoundingBox());
}

SDL_Rect getIntersection(const SDL_Rect& a, const SDL_Rect& b)
{
    int x = (a.x > b.x) ? a.x : b.x;
    int y = (a.y > b.y) ? a.y : b.y;
    int w = (a.x + a.w) < (b.x + b.w) ? (a.x + a.w) - x : (b.x + b.w) - x;
    int h = (a.y + a.h) < (b.y + b.h) ? (a.y + a.h) - y : (b.y + b.h) - y;
    SDL_Rect intersection = { x, y, w, h };
    return intersection;
}
SDL_Rect getIntersection(const Rectangle& a, const Rectangle& b)
{
    return getIntersection(a.getBoundingBox(), b.getBoundingBox());
}

enum class Side { TOP, LEFT, RIGHT, BOTTOM };
Vector2i getCenter(const SDL_Rect& rect)
{
    Vector2i center = {
        rect.x + (rect.w / 2),
        rect.y + (rect.h / 2)
    };
    return center;
}
Vector2i getCenter(const Rectangle& rect)
{
    return getCenter(rect.getBoundingBox());
}

void handlePaddleCollision(Rectangle& ball, const Rectangle& paddle, float dt)
{
    if (checkCollision(ball, paddle))
    {
        Vector2i paddleCenter = getCenter(paddle);
        Vector2i ballCenter = getCenter(ball);
        Vector2f paddleToBall = {
            (float)ballCenter.x - paddleCenter.x,
            (float)ballCenter.y - paddleCenter.y
        };
        float length = std::sqrt((paddleToBall.x * paddleToBall.x) + (paddleToBall.y * paddleToBall.y));
        Vector2f velocity = {
            200 * paddleToBall.x / length,
            200 * paddleToBall.y / length
        };
        ball.setVelocity(velocity);
        ball.update(dt);
    }
}

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
    Rectangle paddle(600.f, 20.f, 25, 200, 0.f, 0.f);
    Rectangle paddle2(50.f, 20.f, 25, 200, 0.f, 0.f);

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

        handlePaddleCollision(dot, paddle, dt);
        handlePaddleCollision(dot, paddle2, dt);

        SDL_SetRenderDrawColor(pRenderer.get(), 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(pRenderer.get());

        dot.draw(pRenderer);
        paddle.draw(pRenderer);
        paddle2.draw(pRenderer);

        SDL_RenderPresent(pRenderer.get());
        t0 = now;
    }

    return 0;
}
