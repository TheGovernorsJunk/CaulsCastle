#include "platformer_physics_system.h"
#include "physics_component.h"
#include "transform_component.h"
#include <glm/gtx/transform.hpp>

namespace te
{
    PlatformerPhysicsSystem::PlatformerPhysicsSystem(
        std::shared_ptr<PhysicsComponent> pPhysics,
        std::shared_ptr<TransformComponent> pTransform,
        float gravityAcceleration)
        : mpPhysics(pPhysics)
        , mpTransform(pTransform)
        , mGravityAcceleration(0, gravityAcceleration)
    {}

    void PlatformerPhysicsSystem::update(float dt) const
    {
        TransformComponent& transformComponent = *mpTransform;
        const glm::vec2& gravityAcceleration = mGravityAcceleration;
        mpPhysics->forEach([&transformComponent, &gravityAcceleration, dt](const Entity& entity, PhysicsInstance& instance)
        {
            instance.velocity += dt * gravityAcceleration;
            transformComponent.setLocalTransform(
                entity,
                glm::translate(transformComponent.getLocalTransform(entity), dt * glm::vec3(instance.velocity.x, instance.velocity.y, 0)));
        });
    }
}
