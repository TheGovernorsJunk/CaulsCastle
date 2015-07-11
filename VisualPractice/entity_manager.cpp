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
}
