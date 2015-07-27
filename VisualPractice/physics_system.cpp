#include "physics_system.h"
#include "physics_component.h"
#include "transform_component.h"
#include <glm/gtx/transform.hpp>

namespace te
{
    PhysicsSystem::PhysicsSystem(
        std::shared_ptr<PhysicsComponent> pPhysics,
        std::shared_ptr<TransformComponent> pTransform)
        : mpPhysics(pPhysics), mpTransform(pTransform) {}

    void PhysicsSystem::update(float dt) const
    {
        TransformComponent& transformComponent = *mpTransform;
        mpPhysics->forEach([&transformComponent, dt](const Entity& entity, PhysicsInstance& instance)
        {
            transformComponent.setLocalTransform(
                entity,
                glm::translate(transformComponent.getLocalTransform(entity), dt * glm::vec3(instance.velocity.x, instance.velocity.y, 0)));
        });
    }
}
