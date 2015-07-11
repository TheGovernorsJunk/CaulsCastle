#include "entity_manager.h"
#include <cassert>

namespace te
{
    EntityManager::EntityManager(unsigned size)
        : mEntities()
    {
        mEntities.reserve(size);
    }

    Entity EntityManager::create()
    {
        if (mEntities.size() < mEntities.capacity() || mAvailableIndices.empty())
        {
            Entity entity;
            entity.index = mEntities.size();
            entity.generation = 0;
            mEntities.push_back(entity);
            return entity;
        }
        else
        {
            Entity entity = mEntities[mAvailableIndices.front()];
            mAvailableIndices.pop_front();
            return entity;
        }
    }

    bool EntityManager::isAlive(Entity entity) const
    {
        return entity.generation == mEntities[entity.index].generation;
    }

    void EntityManager::destroy(Entity entity)
    {
        ++mEntities[entity.index].generation;
        mAvailableIndices.push_back(entity.index);
    }

    /*
    EntityManager::EntityManager()
        : mEntities(1024)
        , mFirstAvailable(&mEntities[0])
        , mLastAvailable(&mEntities[mEntities.size() - 1])
    {
        for (auto it = mEntities.begin(); it != mEntities.end(); ++it)
        {
            Entity& entity = *it;
            entity.index = it - mEntities.begin();
            entity.generation = 0;
            if (it != mEntities.end() - 1)
            {
                entity.next = &*(it + 1);
            }
        }
        mEntities[mEntities.size() - 1].next = nullptr;
    }

    Entity EntityManager::create()
    {
        if (mFirstAvailable != nullptr)
        {
            Entity& entity = *mFirstAvailable;
            mFirstAvailable = entity.next;
            return entity;
        }
        else
        {
            mEntities.resize(mEntities.size() + 1024);
        }
    }

    bool EntityManager::isAlive(Entity entity)
    {
        return mEntities[entity.index].generation == entity.generation;
    }

    void EntityManager::destroy(Entity entity)
    {
        Entity& indexedEntity = mEntities[entity.index];
        ++indexedEntity.generation;
        (*mLastAvailable).next = &indexedEntity;
        mLastAvailable = &indexedEntity;
    }
    */
}
