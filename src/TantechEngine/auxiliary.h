#ifndef TE_AUXILIARY_H
#define TE_AUXILIARY_H

#include <map>
#include "gl.h"
#include <glm/glm.hpp>
#include "types.h"

struct SDL_Rect;

namespace te
{

    //class Rectangle;

    template <typename T>
    bool checkCollision(const T& a, const T& b)
    {
        auto topA = a.y;
        auto leftA = a.x;
        auto bottomA = a.y + a.h;
        auto rightA = a.x + a.w;

        auto topB = b.y;
        auto leftB = b.x;
        auto bottomB = b.y + b.h;
        auto rightB = b.x + b.w;

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
    //bool checkCollision(const glm::vec2 a[4] , const glm::vec2 b[4]);

    template <typename T>
    T getIntersection(const T& a, const T& b)
    {
        auto x = (a.x > b.x) ? a.x : b.x;
        auto y = (a.y > b.y) ? a.y : b.y;
        auto w = (a.x + a.w) < (b.x + b.w) ? (a.x + a.w) - x : (b.x + b.w) - x;
        auto h = (a.y + a.h) < (b.y + b.h) ? (a.y + a.h) - y : (b.y + b.h) - y;
        T intersection = { x, y, w, h };
        return intersection;
    }

    Vector2i getCenter(const SDL_Rect& rect);

    // void handlePaddleCollision(Rectangle& ball, const Rectangle& paddle, float dt, float velocityScalar = 200.f);
    // void handleWallCollision(Rectangle& ball, const Rectangle& wall, float dt);

    template <class K, class V>
    void insertOrAssign(std::map<K,V>& map, std::pair<K, V>&& kvPair)
    {
        auto it = map.find(kvPair.first);
        if (it == map.end())
        {
            map.insert(std::move(kvPair));
        }
        else
        {
            it->second = std::move(kvPair.second);
        }
    }

    template <class K, class V>
    void insertOrAssign(std::map<K, V>& map, const std::pair<K, V>& kvPair)
    {
        auto it = map.find(kvPair.first);
        if (it != map.end())
        {
            map.insert(kvPair);
        }
        else
        {
            it->second = kvPair.second;
        }
    }

    void adjustViewport(int screenWidth, int screenHeight, float targetAspectRatio, void(__stdcall* viewportFn)(GLint, GLint, GLsizei, GLsizei));
}

#endif
