#ifndef TE_ENTITY_MANAGER_H
#define TE_ENTITY_MANAGER_H

#include "observer.h"
#include <vector>
#include <deque>
#include <memory>

namespace te
{
    class Entity
    {
    public:
        bool operator<(const Entity&) const;
        bool operator==(const Entity&) const;
        bool operator!=(const Entity&) const;
    private:
        friend class EntityManager;
        unsigned index;
        unsigned generation;
    };

    struct DestroyEvent
    {
        Entity entity;
    };

    class EntityManager
    {
    public:
        typedef std::vector<std::shared_ptr<Observer<DestroyEvent>>> ObserverVector;

        EntityManager(ObserverVector&& observers = {}, unsigned size = 1024);
        Entity create();
        bool isAlive(Entity e) const;
        void destroy(Entity e);
    private:
        EntityManager(const EntityManager&) = delete;
        EntityManager& operator=(const EntityManager&) = delete;

        typedef std::vector<Entity> EntityContainer;
        typedef std::deque<unsigned> IndexQueue;

        EntityContainer mEntities;
        IndexQueue mAvailableIndices;
        unsigned mAllocationSize;

        ObserverVector mObservers;
    };
}

#endif /* TE_ENTITY_H */
