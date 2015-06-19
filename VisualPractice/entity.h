#ifndef TE_ENTITY_H
#define TE_ENTITY_H

#include "wrappers.h"

namespace te
{

    struct Vector2f;

    class Entity
    {
    public:
        Entity(float x, float y, float dx = 0, float dy = 0);
        virtual ~Entity();

        Vector2f getVelocity() const;
        void setVelocity(Vector2f vel);

        Vector2f getPosition() const;
        void setPosition(Vector2f pos);

        virtual void update(float dt);

        virtual void draw(RendererPtr) const = 0;
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
        Rectangle(float x, float y, int w, int h, float dx = 0.f, float dy = 0.f);

        virtual SDL_Rect getBoundingBox() const;
        virtual void draw(RendererPtr) const;
    private:
        int mW;
        int mH;
    };
}

#endif
