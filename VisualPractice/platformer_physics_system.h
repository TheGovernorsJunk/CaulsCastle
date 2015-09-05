#ifndef TE_PLATFORMER_PHYSICS_H
#define TE_PLATFORMER_PHYSICS_H

#include <memory>
#include <glm/glm.hpp>

namespace te
{
    class PhysicsComponent;
    class TransformComponent;

    class PlatformerPhysicsSystem
    {
    public:
        PlatformerPhysicsSystem(
            std::shared_ptr<PhysicsComponent> pPhysics,
            std::shared_ptr<TransformComponent> pTransform,
            float gravityAcceleration);

        void update(float dt) const;

    private:
        std::shared_ptr<PhysicsComponent> mpPhysics;
        std::shared_ptr<TransformComponent> mpTransform;
        glm::vec2 mGravityAcceleration;
    };
}

#endif /* TE_PLATFORMER_PHYSICS_H */
