#include "entity_manager.h"
#include <cassert>

namespace te
{
    Entity::Entity(unsigned id)
        : selfOrNext(id) {}

    EntityManager::EntityManager(size_t size)
        : mGeneration(size, Entity(0))
        , mFirstAvailable(0)
    {
        assert(size > 0);
        for (unsigned i = 0; i < mGeneration.size(); ++i)
        {
            mGeneration[i].selfOrNext = i + 1;
        }
    }

    Entity EntityManager::create()
    {
        Entity& entity = mGeneration[mFirstAvailable];
        std::swap(entity.selfOrNext, mFirstAvailable);
        if (mFirstAvailable == mGeneration.size())
        {
            mGeneration.resize(
                mGeneration.size() + DEFAULT_SIZE,
                0);
            for (unsigned i = mFirstAvailable; i < mGeneration.size(); ++i)
            {
                mGeneration[i].selfOrNext = i + 1;
            }
        }
        return entity;
    }

    bool EntityManager::isAlive(Entity entity)
    {
        return mGeneration[entity.selfOrNext].selfOrNext == entity.selfOrNext;
    }

    void EntityManager::destroy(Entity entity)
    {
        if (isAlive(entity))
        {
            std::swap(mGeneration[entity.selfOrNext].selfOrNext, mFirstAvailable);
        }
    }
}
