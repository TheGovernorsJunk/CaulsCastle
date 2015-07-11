#ifndef TE_TYPES_H
#define TE_TYPES_H

#include <array>
#include <iosfwd>
#include <cmath>

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

    template<typename T, typename O>
    Vector2<T> convertVector2(O o)
    {
        return Vector2<T>((T)o.x, (T)o.y);
    }

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

    template<typename T>
    class Transform3
    {
    public:
        typedef std::array<T, 9> Matrix;

        Transform3()
            : mMatrix()
        {
            mMatrix[0] = (T)1; mMatrix[3] = (T)0; mMatrix[6] = (T)0;
            mMatrix[1] = (T)0; mMatrix[4] = (T)1; mMatrix[7] = (T)0;
            mMatrix[2] = (T)0; mMatrix[5] = (T)0; mMatrix[8] = (T)1;
        }

        Transform3(T a00, T a01, T a02,
                   T a10, T a11, T a12,
                   T a20, T a21, T a22)
            : mMatrix()
        {
            mMatrix[0] = a00; mMatrix[3] = a01; mMatrix[6] = a02;
            mMatrix[1] = a10; mMatrix[4] = a11; mMatrix[7] = a12;
            mMatrix[2] = a20; mMatrix[5] = a21; mMatrix[8] = a22;
        }

        Vector2<T> transformPoint(T x, T y) const
        {
            return Vector2<T>(mMatrix[0] * x + mMatrix[3] * y + mMatrix[6],
                              mMatrix[1] * x + mMatrix[4] * y + mMatrix[7]);
        }

        Vector2<T> transformPoint(Vector2<T> point) const
        {
            return transformPoint(point.x, point.y);
        }

        Transform3& combine(const Transform3& transform)
        {
            const Matrix& a = mMatrix;
            const Matrix& b = transform.mMatrix;

            *this = Transform3(
                a[0] * b[0] + a[3] * b[1] + a[6] * b[2], a[0] * b[3] + a[3] * b[4] + a[6] * b[5], a[0] * b[6] + a[3] * b[7] + a[6] * b[8],
                a[1] * b[0] + a[4] * b[1] + a[7] * b[2], a[1] * b[3] + a[4] * b[4] + a[7] * b[5], a[1] * b[6] + a[4] * b[7] + a[7] * b[8],
                a[2] * b[0] + a[5] * b[1] + a[8] * b[2], a[2] * b[3] + a[5] * b[4] + a[8] * b[5], a[2] * b[6] + a[5] * b[7] + a[8] * b[8]);

            return *this;
        }

        Transform3& translate(T x, T y)
        {
            Transform3 translation(1, 0, x,
                                   0, 1, y,
                                   0, 0, 1);
            return combine(translation);
        }

        Transform3& translate(const Vector2<T> offset)
        {
            return translate(offset.x, offset.y);
        }

        Transform3& rotate(float angle)
        {
            float radians = angle * (float)M_PI / 180.f;
            float cos = std::cos(radians);
            float sin = std::sin(radians);

            Transform3 rotation((T)cos, (T)-sin, 0,
                                (T)sin, (T) cos, 0,
                                     0,       0, 1);

            return combine(rotation);
        }

        Transform3& rotate(float angle, T centerX, T centerY)
        {
            float radians = angle * (float)M_PI / 180.f;
            float cos = std::cos(radians);
            float sin = std::sin(radians);

            Transform3 rotation((T)cos, (T)-sin, (T)((float)centerX * (1.f - cos) + (float)centerY * sin),
                                (T)sin, (T) cos, (T)((float)centerY * (1.f - cos) + (float)centerX * sin),
                                     0,       0,                                                       1);

            return combine(rotation);
        }

        Transform3& rotate(float angle, const Vector2<T>& center)
        {
            return rotate(angle, center.x, center.y);
        }

        Transform3& scale(T scaleX, T scaleY)
        {
            Transform3 scaling(scaleX,      0, 0,
                                    0, scaleY, 0,
                                    0,      0, 1);
            return combine(scaling);
        }

        Transform3& scale(T scaleX, T scaleY, T centerX, T centerY)
        {
            Transform3 scaling(scaleX,      0, centerX * (1 - scaleX),
                                    0, scaleY, centerY * (1 - scaleY),
                                    0,      0,                      1);

            return combine(scaling);
        }

        Transform3& scale(const Vector2<T>& factors)
        {
            return scale(factors.x, factors.y);
        }

        Transform3& scale(const Vector2<T>& factors, const Vector2<T>& center)
        {
            return scale(factors.x, factors.y, center.x, center.y);
        }

    private:
        friend std::ostream& operator<< (std::ostream& out, Transform3& transform)
        {
            Matrix& t = transform.mMatrix;
            out << t[0] << '\t' << t[3] << '\t' << t[6] << '\n'
                << t[1] << '\t' << t[4] << '\t' << t[7] << '\n'
                << t[2] << '\t' << t[5] << '\t' << t[8] << '\n';
            return out;
        }
        Matrix mMatrix;
    };

    template<typename T>
    Transform3<T> operator*(const Transform3<T>& left, const Transform3<T>& right)
    {
        return Transform3<T>(left).combine(right);
    }

    template<typename T>
    Transform3<T>& operator*=(Transform3<T>& left, const Transform3<T>& right)
    {
        return left.combine(right);
    }

    template<typename T>
    Vector2<T> operator*(const Transform3<T>& left, const Vector2<T>& right)
    {
        return left.transformPoint(right);
    }
}

#endif
