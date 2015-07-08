#ifndef TE_ENTITY_H
#define TE_ENTITY_H

#include <vector>

namespace te
{
    class Entity
    {
    private:
        friend class EntityManager;
        unsigned selfOrNext;
        Entity(unsigned id);
    };

    class EntityManager
    {
    public:
        EntityManager(size_t size = DEFAULT_SIZE);
        Entity create();
        bool isAlive(Entity e);
        void destroy(Entity e);
    private:
        const static size_t DEFAULT_SIZE = 1024;
        std::vector<Entity> mGeneration;
        unsigned mFirstAvailable;
    };
}

#endif /* TE_ENTITY_H */
