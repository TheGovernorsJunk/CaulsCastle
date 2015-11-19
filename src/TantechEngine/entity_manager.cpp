#include "entity_manager.h"
#include <cassert>
#include <algorithm>
#include <iostream>

namespace te
{
    bool Entity::operator<(const Entity& o) const
    {
        return index == o.index ?
            generation < o.generation :
            index < o.index;
    }

    bool Entity::operator==(const Entity& o) const
    {
        return ((index == o.index) && (generation == o.generation));
    }

    bool Entity::operator!=(const Entity& o) const
    {
        return !(*this == o);
    }

    std::ostream& operator<<(std::ostream& out, const Entity& e)
    {
        out << e.index << ":" << e.generation;
        return out;
    }

    EntityManager::EntityManager(ObserverVector&& observers, unsigned size)
        : mEntities()
        , mAvailableIndices()
        , mAllocationSize(size)
        , mObservers(std::move(observers))
    {
        mEntities.reserve(size);
    }

    Entity EntityManager::create()
    {
        if (mEntities.size() < mEntities.capacity() || mAvailableIndices.empty())
        {
            if (mEntities.size() == mEntities.capacity())
            {
                mEntities.reserve(mEntities.capacity() + mAllocationSize);
            }
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

        DestroyEvent evt{ entity };
        std::for_each(
            std::begin(mObservers),
            std::end(mObservers),
            [evt](std::shared_ptr<Observer<DestroyEvent>>& observer)
        {
            observer->onNotify(evt);
        });
    }
}
