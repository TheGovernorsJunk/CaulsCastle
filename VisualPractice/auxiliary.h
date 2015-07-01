#ifndef TE_AUXILIARY_H
#define TE_AUXILIARY_H

#include <map>
#include "types.h"

struct SDL_Rect;

namespace te
{

    class Rectangle;

    bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
    bool checkCollision(const Rectangle& a, const Rectangle& b);

    SDL_Rect getIntersection(const SDL_Rect& a, const SDL_Rect& b);
    SDL_Rect getIntersection(const Rectangle& a, const Rectangle& b);

    Vector2i getCenter(const SDL_Rect& rect);
    Vector2i getCenter(const Rectangle& rect);

    void handlePaddleCollision(Rectangle& ball, const Rectangle& paddle, float dt, float velocityScalar = 200.f);
    void handleWallCollision(Rectangle& ball, const Rectangle& wall, float dt);

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
}

#endif