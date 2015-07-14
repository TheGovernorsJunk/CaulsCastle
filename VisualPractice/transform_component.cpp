#include "transform_component.h"

namespace te
{
    TransformComponent::TransformComponent(std::size_t capacity)
        : mData()
        , mMap()
    {
        mData.reserve(capacity);
    }

    unsigned TransformComponent::createInstance(const Entity& entity)
    {
        if (mData.size() == mData.capacity())
        {
            mData.reserve(mData.capacity() + 1024);
        }
        Instance instance;

        instance.entity = entity;
        instance.local = Transform3<float>();
        instance.parent = entity;
        instance.firstChild = entity;
        instance.nextSibling = entity;
        instance.prevSibling = entity;

        unsigned i = mData.size();
        mData.push_back(std::move(instance));
        mMap.insert(std::make_pair(entity, i));

        return i;
    }

    void TransformComponent::setParent(const Entity& child, const Entity& parent)
    {
        unsigned i = getEntityIndex(child);
        mData[i].parent = parent;
        Transform3<float> parentTransform = mData[getEntityIndex(parent)].world;
        transformTree(i, parentTransform);
    }

    void TransformComponent::setLocalTransform(const Entity& entity, const Transform3<float>& transform)
    {
        unsigned i = getEntityIndex(entity);
        mData[i].local = transform;
        Transform3<float> parentTransform =
            mData[i].parent != mData[i].entity ?
            mData[getEntityIndex(mData[i].parent)].local :
            Transform3<float>();
        transformTree(i, parentTransform);
    }

    Transform3<float> TransformComponent::getWorldTransform(const Entity& entity) const
    {
        auto it = mMap.find(entity);
        if (it != mMap.end())
        {
            return mData[it->second].world;
        }
        return Transform3<float>();
    }

    void TransformComponent::transformTree(unsigned i, Transform3<float> parentTransform)
    {
        mData[i].world = parentTransform.combine(mData[i].local);

        unsigned childIndex = getEntityIndex(mData[i].firstChild);
        while (childIndex != i)
        {
            transformTree(childIndex, mData[i].local);
            if (mData[childIndex].nextSibling != mData[childIndex].entity)
            {
                childIndex = getEntityIndex(mData[childIndex].nextSibling);
            }
            else
            {
                childIndex = i;
            }
        }
    }

    unsigned TransformComponent::getEntityIndex(const Entity& entity)
    {
        auto it = mMap.find(entity);
        if (it != mMap.end())
        {
            return it->second;
        }
        return createInstance(entity);
    }

    void TransformComponent::destroyInstance(unsigned i)
    {
        unsigned lastI = mData.size() - 1;
        Entity lastEntity = mData[lastI].entity;
        Entity entity = mData[i].entity;

        mData[i] = mData[lastI];

        mMap[lastEntity] = i;
        auto it = mMap.find(entity);
        mMap.erase(it);

        mData.pop_back();
    }
}
