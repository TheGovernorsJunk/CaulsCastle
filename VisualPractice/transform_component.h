#ifndef TE_TRANSFORM_COMPONENT_H
#define TE_TRANSFORM_COMPONENT_H

#include <map>
#include "entity_manager.h"
#include "types.h"

namespace te
{
    class Instance
    {
    private:
        friend class TransformComponent;
        Instance(unsigned i);
        unsigned i;
    };

    class TransformComponent
    {
    public:
        TransformComponent(std::size_t capacity = 1024);
        ~TransformComponent();

        void reserve(std::size_t capacity);

        Instance createInstance();
        void destroyInstance(Instance instance);
    private:
        struct InstanceData
        {
            std::size_t size;
            std::size_t capacity;
            void* buffer;

            Entity* entity;
            Transform3<float>* local;
            Transform3<float>* world;
        };
        typedef std::map<Entity, unsigned> InstanceMap;

        InstanceData mData;
        InstanceMap mMap;
    };
}

#endif
