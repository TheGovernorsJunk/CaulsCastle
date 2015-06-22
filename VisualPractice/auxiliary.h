#ifndef TE_AUXILIARY_H
#define TE_AUXILIARY_H

struct SDL_Rect;

namespace te
{

    struct Vector2i;
    class Rectangle;

    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
    bool checkCollision(const Rectangle& a, const Rectangle& b);

    SDL_Rect getIntersection(const SDL_Rect& a, const SDL_Rect& b);
    SDL_Rect getIntersection(const Rectangle& a, const Rectangle& b);

    Vector2i getCenter(const SDL_Rect& rect);
    Vector2i getCenter(const Rectangle& rect);

    void handlePaddleCollision(Rectangle& ball, const Rectangle& paddle, float dt, float velocityScalar = 200.f);
    void handleWallCollision(Rectangle& ball, const Rectangle& wall, float dt);
}

#endif
