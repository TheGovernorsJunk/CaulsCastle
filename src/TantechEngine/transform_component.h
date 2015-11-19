#ifndef TE_TRANSFORM_COMPONENT_H
#define TE_TRANSFORM_COMPONENT_H

#include <functional>
#include <map>
#include <memory>
#include <glm/glm.hpp>
#include "entity_manager.h"
#include "component.h"

namespace te
{
    struct TransformInstance
    {
        glm::mat4 local;
        glm::mat4 world;
        Entity parent;
        Entity firstChild;
        Entity nextSibling;
        Entity prevSibling;
    };

    class TransformComponent : public Component<TransformInstance>
    {
    public:
        TransformComponent(std::size_t capacity = 1024);

        void setParent(const Entity& child, const Entity& parent);
        void setLocalTransform(const Entity& entity, const glm::mat4& transform);
        glm::mat4 getWorldTransform(const Entity& entity) const;
        glm::mat4 getLocalTransform(const Entity& entity) const;

    private:
        TransformComponent(const TransformComponent&) = delete;
        TransformComponent& operator=(const TransformComponent&) = delete;

        void transformTree(TransformInstance& instance, const glm::mat4& parentTransform);
    };

    typedef std::shared_ptr<TransformComponent> TransformPtr;
}

#endif
