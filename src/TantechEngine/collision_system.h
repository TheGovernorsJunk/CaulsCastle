#ifndef TE_COLLISION_SYSTEM_H
#define TE_COLLISION_SYSTEM_H

#include <vector>
#include <memory>
#include "entity_manager.h"
#include "observer.h"

namespace te
{
    class BoundingBoxComponent;
    class TransformComponent;
    class TiledMap;

    struct CollisionEvent
    {
        Entity a;
        Entity b;
        float dt;
    };

    class CollisionSystem
    {
    public:
        typedef std::vector<std::shared_ptr<Observer<CollisionEvent>>> ObserverList;

        CollisionSystem(
            std::shared_ptr<BoundingBoxComponent> pBoundingBox,
            ObserverList&& observers);

        void update(float dt) const;

    private:
        std::shared_ptr<BoundingBoxComponent> mpBoundingBox;
        ObserverList mObservers;
    };

    struct MapCollisionEvent
    {
        Entity entity;
        TiledMap& map;
        float dt;
    };

    class MapCollisionSystem
    {
    public:
        typedef std::vector<std::shared_ptr<Observer<MapCollisionEvent>>> ObserverList;

        MapCollisionSystem(
            std::shared_ptr<BoundingBoxComponent> pBoundingBox,
            std::shared_ptr<TiledMap> pTiledMap,
            ObserverList&& observers);

        void update(float dt) const;

    private:
        std::shared_ptr<BoundingBoxComponent> mpBoundingBox;
        std::shared_ptr<TiledMap> mpTiledMap;
        ObserverList mObservers;
    };
}

#endif
