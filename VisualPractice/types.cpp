#include "types.h"

namespace te
{

    Vector2f::Vector2f(float x, float y)
        : x(x), y(y) {}

    Vector2f Vector2f::operator+(Vector2f o)
    {
        return Vector2f(x + o.x, y + o.y);
    }

    Vector2f Vector2f::operator-(Vector2f o)
    {
        return Vector2f(x - o.x, y - o.y);
    }

    Vector2i::Vector2i(int x, int y)
        : x(x), y(y) {}

    Vector2i Vector2i::operator+(Vector2i o)
    {
        return Vector2i(x + o.x, y + o.y);
    }

    Vector2i Vector2i::operator-(Vector2i o)
    {
        return Vector2i(x - o.x, y - o.y);
    }
}
