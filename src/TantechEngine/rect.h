#ifndef TE_RECT_H
#define TE_RECT_H

namespace te
{
    template <typename T> struct Rect
    {
        T x; T y; T w; T h;
        Rect() : x(0), y(0), w(0), h(0) {}
        Rect(T x, T y, T w, T h)
            : x(x), y(y), w(w), h(h) {}
    };

    typedef Rect<float> FloatRect;
    typedef Rect<int> IntRect;
}

#endif
