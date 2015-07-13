#include "transform_component.h"

namespace te
{
    TransformComponent::TransformComponent(std::size_t capacity)
        : mData()
        , mMap()
    {
        mData = {0,0,nullptr,nullptr,nullptr,nullptr};
        reserve(capacity);
    }

    TransformComponent::~TransformComponent()
    {
        ::operator delete(mData.buffer);
    }

    Instance TransformComponent::createInstance()
    {
        if (mData.size == mData.capacity)
        {
            reserve(mData.capacity + 1024);
        }
        return Instance(mData.size++);
    }

    void TransformComponent::destroyInstance(Instance instance)
    {

    }

    void TransformComponent::reserve(std::size_t capacity)
    {
        if (capacity <= mData.capacity) return;

        InstanceData data;

        const std::size_t bytes = capacity * (sizeof(Entity) + 2 * sizeof(Transform3<float>));
        data.buffer = static_cast<void*>(::operator new(bytes));
        data.size = mData.size;
        data.capacity = capacity;

        data.entity = (Entity*)(data.buffer);
        data.local = (Transform3<float>*)(data.entity + capacity);
        data.world = (Transform3<float>*)(data.local + capacity);

        std::memcpy(data.entity, mData.entity, mData.size * sizeof(Entity));
        std::memcpy(data.local, mData.local, mData.size * sizeof(Transform3<float>));
        std::memcpy(data.world, mData.world, mData.size * sizeof(Transform3<float>));

        ::operator delete(mData.buffer);
        mData = data;
    }
}
