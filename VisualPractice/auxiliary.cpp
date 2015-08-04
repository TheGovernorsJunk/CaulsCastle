#include "auxiliary.h"
#include <SDL_rect.h>
#include "types.h"

namespace te
{

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

    SDL_Rect getIntersection(const SDL_Rect& a, const SDL_Rect& b)
    {
        int x = (a.x > b.x) ? a.x : b.x;
        int y = (a.y > b.y) ? a.y : b.y;
        int w = (a.x + a.w) < (b.x + b.w) ? (a.x + a.w) - x : (b.x + b.w) - x;
        int h = (a.y + a.h) < (b.y + b.h) ? (a.y + a.h) - y : (b.y + b.h) - y;
        SDL_Rect intersection = { x, y, w, h };
        return intersection;
    }
    
    Vector2i getCenter(const SDL_Rect& rect)
    {
        Vector2i center = {
            rect.x + (rect.w / 2),
            rect.y + (rect.h / 2)
        };
        return center;
    }

    //void handlePaddleCollision(Rectangle& ball, const Rectangle& paddle, float dt, float velocityScalar)
    //{
    //    if (checkCollision(ball, paddle))
    //    {
    //        Vector2i paddleCenter = getCenter(paddle);
    //        Vector2i ballCenter = getCenter(ball);
    //        Vector2f paddleToBall = {
    //            (float)ballCenter.x - paddleCenter.x,
    //            (float)ballCenter.y - paddleCenter.y
    //        };
    //        float length = std::sqrt((paddleToBall.x * paddleToBall.x) + (paddleToBall.y * paddleToBall.y));
    //        Vector2f velocity = {
    //            velocityScalar * paddleToBall.x / length,
    //            velocityScalar * paddleToBall.y / length
    //        };
    //        ball.setVelocity(velocity);
    //        ball.update(dt);
    //    }
    //}

    //void handleWallCollision(Rectangle& ball, const Rectangle& wall, float dt)
    //{
    //    if (checkCollision(ball, wall))
    //    {
    //        Vector2f vel = ball.getVelocity();
    //        SDL_Rect intersection = getIntersection(ball, wall);
    //        if (intersection.w > intersection.h)
    //        {
    //            vel.y = -vel.y;
    //            ball.setVelocity(vel);
    //        }
    //        else
    //        {
    //            vel.x = -vel.x;
    //            ball.setVelocity(vel);
    //        }
    //    }
    //}
}
