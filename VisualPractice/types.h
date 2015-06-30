#ifndef TE_TYPES_H
#define TE_TYPES_H

namespace te
{

    template <typename T>
    struct Vector2
    {
        T x;
        T y;
        Vector2(T x = 0, T y = 0)
            : x(x), y(y) {}
        Vector2<T>& operator+=(Vector2<T> o)
        {
            x += o.x;
            y += o.y;
            return *this;
        }
    };

    template <typename T>
    Vector2<T> operator+(Vector2<T> a, Vector2<T> b)
    {
        return Vector2<T>(a.x + b.x, a.y + b.y);
    }

    template <typename T>
    Vector2<T> operator-(Vector2<T> a, Vector2<T> b)
    {
        return Vector2<T>(a.x - b.x, a.y - b.y);
    }

    template <typename T>
    Vector2<T> operator*(Vector2<T> vector, T scalar)
    {
        return Vector2<T>(vector.x * scalar, vector.y * scalar);
    }

    template <typename T>
    Vector2<T> operator*(T scalar, Vector2<T> vector)
    {
        return Vector2<T>(vector.x * scalar, vector.y * scalar);
    }

    template <typename T>
    Vector2<T> operator/(Vector2<T> vector, T scalar)
    {
        return Vector2<T>(vector.x / scalar, vector.y / scalar);
    }

    template<typename T>
    T length(Vector2<T> v)
    {
        return std::sqrt((v.x * v.x) + (v.y * v.y));
    }

    template<typename T>
    Vector2<T> normalize(Vector2<T> v)
    {
        return v / length(v);
    }

    typedef Vector2<float> Vector2f;
    typedef Vector2<int> Vector2i;
}

#endif
