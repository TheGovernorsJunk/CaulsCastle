#ifndef TE_TRANSFORM_COMPONENT_H
#define TE_TRANSFORM_COMPONENT_H

#include <map>
#include "entity_manager.h"
#include "types.h"

namespace te
{

    class TransformComponent
    {
    public:
        TransformComponent(std::size_t capacity = 1024);

        void setParent(const Entity& child, const Entity& parent);
        void setLocalTransform(const Entity& entity, const Transform3<float>& transform);
        Transform3<float> getWorldTransform(const Entity& entity) const;

        void reserve(std::size_t capacity);

    private:
        struct Instance
        {
            Entity entity;
            Transform3<float> local;
            Transform3<float> world;
            Entity parent;
            Entity firstChild;
            Entity nextSibling;
            Entity prevSibling;
        };
        typedef std::map<Entity, unsigned> InstanceMap;

        unsigned createInstance(const Entity& entity);
        unsigned getEntityIndex(const Entity& entity);
        void destroyInstance(unsigned instance);

        void transformTree(unsigned index, Transform3<float> parentTransform);

        std::vector<Instance> mData;
        InstanceMap mMap;
    };
}

#endif
