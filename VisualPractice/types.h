#ifndef TE_TYPES_H
#define TE_TYPES_H

namespace te
{

    struct Vector2f
    {
        float x;
        float y;
        Vector2f(float x = 0, float y = 0);
        Vector2f operator+(Vector2f o);
        Vector2f operator-(Vector2f o);
    };

    struct Vector2i
    {
        int x;
        int y;
        Vector2i(int x = 0, int y = 0);
        Vector2i operator+(Vector2i o);
        Vector2i operator-(Vector2i o);
    };

}

#endif
