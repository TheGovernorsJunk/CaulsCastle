#include "physics_component.h"

namespace te
{
    PhysicsComponent::PhysicsComponent(size_t capacity)
        : Component(capacity) {}

    void PhysicsComponent::setPhysics(const Entity& entity, const glm::vec2& velocity)
    {
        if (!hasInstance(entity))
        {
            createInstance(entity, { velocity });
        }
        else
        {
            at(entity).velocity = velocity;
        }
    }

    glm::vec2 PhysicsComponent::getVelocity(const Entity& entity) const
    {
        if (hasInstance(entity)) { return at(entity).velocity; }
        else { return{ 0, 0 }; }
    }
}
