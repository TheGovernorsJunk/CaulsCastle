#ifndef TE_PHYSICS_COMPONENT_H
#define TE_PHYSICS_COMPONENT_H

#include <memory>
#include <glm/glm.hpp>
#include "component.h"
#include "entity_manager.h"

namespace te
{
    struct PhysicsInstance
    {
        glm::vec2 velocity;
    };

    class PhysicsComponent : public Component<PhysicsInstance>
    {
    public:
        PhysicsComponent(size_t capacity = 1024);

        void setPhysics(const Entity& entity, const glm::vec2& velocity = {0,0});
        glm::vec2 getVelocity(const Entity& entity) const;
    private:
        friend class PhysicsSystem;
    };

    typedef std::shared_ptr<PhysicsComponent> PhysicsPtr;
}

#endif
