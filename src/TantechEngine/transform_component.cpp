#include "transform_component.h"
#include <algorithm>

namespace te
{
    TransformComponent::TransformComponent(std::size_t capacity)
        : Component(capacity) {}

    TransformInstance createTransformInstance(const Entity& entity)
    {
        return TransformInstance{
            glm::mat4(),
            glm::mat4(),
            entity,
            entity,
            entity,
            entity
        };
    }

    void TransformComponent::setParent(const Entity& child, const Entity& parent)
    {
        if (!hasInstance(child)) { createInstance(child, createTransformInstance(child)); }
        TransformInstance& childInstance = at(child);
        childInstance.parent = parent;
        glm::mat4 parentTransform = at(parent).world;
        transformTree(childInstance, parentTransform);
    }

    void TransformComponent::setLocalTransform(const Entity& entity, const glm::mat4& transform)
    {
        if (!hasInstance(entity)) { createInstance(entity, createTransformInstance(entity)); }
        TransformInstance& instance = at(entity);
        instance.local = transform;
        glm::mat4 parentTransform =
            instance.parent != entity ?
            at(instance.parent).world :
            glm::mat4();
        transformTree(instance, parentTransform);
    }

    void TransformComponent::multiplyLocalTransform(const Entity& entity, const glm::mat4& transform)
    {
        if (!hasInstance(entity)) { createInstance(entity, createTransformInstance(entity)); }
        TransformInstance& instance = at(entity);
        instance.local = transform * instance.local;
        glm::mat4 parentTransform =
            instance.parent != entity ?
            at(instance.parent).world :
            glm::mat4();
        transformTree(instance, parentTransform);
    }

    glm::mat4 TransformComponent::getWorldTransform(const Entity& entity) const
    {
        if (hasInstance(entity))
        {
            return at(entity).world;
        }
        else
        {
            return glm::mat4();
        }
    }

    glm::mat4 TransformComponent::getLocalTransform(const Entity& entity) const
    {
        if (hasInstance(entity))
        {
            return at(entity).local;
        }
        else
        {
            return glm::mat4();
        }
    }

    void TransformComponent::transformTree(TransformInstance& instance, const glm::mat4& parentTransform)
    {
        instance.world = parentTransform * instance.local;

        TransformInstance& childInstance = at(instance.firstChild);
        while (&instance != &childInstance)
        {
            transformTree(childInstance, instance.world);
            if (&at(childInstance.nextSibling) != &childInstance)
            {
                childInstance = at(childInstance.nextSibling);
            }
            else
            {
                childInstance = instance;
            }
        }
    }
}
