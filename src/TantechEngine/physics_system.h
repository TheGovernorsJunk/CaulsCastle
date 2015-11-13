#ifndef TE_PHYSICS_SYSTEM_H
#define TE_PHYSICS_SYSTEM_H

#include <memory>

namespace te
{
    class PhysicsComponent;
    class TransformComponent;

    class PhysicsSystem
    {
    public:
        PhysicsSystem(
            std::shared_ptr<PhysicsComponent> pPhysics,
            std::shared_ptr<TransformComponent> pTransform);

        void update(float dt) const;

    private:
        std::shared_ptr<PhysicsComponent> mpPhysics;
        std::shared_ptr<TransformComponent> mpTransform;
    };
}

#endif
