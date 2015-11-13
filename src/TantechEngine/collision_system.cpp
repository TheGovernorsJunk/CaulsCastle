#include "collision_system.h"
#include "bounding_box_component.h"
#include "transform_component.h"
#include "auxiliary.h"
#include "tiled_map.h"
#include <SDL_rect.h>

namespace te
{
    Observer<CollisionEvent>::~Observer() {}

    CollisionSystem::CollisionSystem(
        std::shared_ptr<BoundingBoxComponent> pBoundingBox,
        ObserverList&& observers)
        : mpBoundingBox(pBoundingBox)
        , mObservers(std::move(observers))
    {}

    void CollisionSystem::update(float dt) const
    {
        BoundingBoxComponent& bbcomponent = *mpBoundingBox;
        const ObserverList& observers = mObservers;

        bbcomponent.forEach([&bbcomponent, &observers, dt](const Entity& entityA, BBInstance instanceA)
        {
            bbcomponent.forEach([&entityA, &bbcomponent, &observers, dt](const Entity& entityB, BBInstance& instanceB)
            {
                if (entityA != entityB &&
                    checkCollision(bbcomponent.getBoundingBox(entityA),
                                   bbcomponent.getBoundingBox(entityB)))
                {
                    std::for_each(std::begin(observers), std::end(observers), [&entityA, &entityB, dt](std::shared_ptr<Observer<CollisionEvent>> pObserver)
                    {
                        pObserver->onNotify({entityA, entityB, dt});
                    });
                }
            });
        });
    }

    MapCollisionSystem::MapCollisionSystem(
        std::shared_ptr<BoundingBoxComponent> pBoundingBox,
        std::shared_ptr<TiledMap> pTiledMap,
        ObserverList&& observers)
        : mpBoundingBox(pBoundingBox)
        , mpTiledMap(pTiledMap)
        , mObservers(std::move(observers))
    {}

    void MapCollisionSystem::update(float dt) const
    {
        BoundingBoxComponent& bbcomponent = *mpBoundingBox;
        const ObserverList& observers = mObservers;
        TiledMap& tiledMap = *mpTiledMap;

        bbcomponent.forEach([&bbcomponent, &observers, &tiledMap, dt](const Entity& entity, BBInstance instance)
        {
            if (tiledMap.checkCollision(bbcomponent.getBoundingBox(entity)))
            {
                std::for_each(std::begin(observers), std::end(observers), [entity, &tiledMap, dt](std::shared_ptr<Observer<MapCollisionEvent>> pObserver)
                {
                    pObserver->onNotify({ entity, tiledMap, dt });
                });
            }
        });
    }
}
