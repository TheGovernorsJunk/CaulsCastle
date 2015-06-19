#include "entity.h"
#include "types.h"

namespace te
{

    Entity::Entity(float x, float y, float dx, float dy)
        : mX(x), mY(y), mDx(dx), mDy(dy) {}

    Entity::~Entity() {}

    Vector2f Entity::getVelocity() const
    {
        Vector2f vel = { mDx, mDy };
        return vel;
    }

    void Entity::setVelocity(Vector2f vel)
    {
        mDx = vel.x;
        mDy = vel.y;
    }

    Vector2f Entity::getPosition() const
    {
        Vector2f pos = { mX, mY };
        return pos;
    }

    void Entity::setPosition(Vector2f pos)
    {
        mX = pos.x;
        mY = pos.y;
    }

    void Entity::update(float dt)
    {
        mX += mDx * dt;
        mY += mDy * dt;
    }

    Rectangle::Rectangle(float x, float y, int w, int h, float dx, float dy)
        : Entity(x, y, dx, dy), mW(w), mH(h) {}

    SDL_Rect Rectangle::getBoundingBox() const
    {
        Vector2f pos = getPosition();
        SDL_Rect rect = { (int)pos.x, (int)pos.y, mW, mH };
        return rect;
    }

    void Rectangle::draw(te::RendererPtr pRenderer) const
    {
        SDL_SetRenderDrawColor(pRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_Rect rect = getBoundingBox();
        SDL_RenderFillRect(pRenderer.get(), &rect);
    }
}
