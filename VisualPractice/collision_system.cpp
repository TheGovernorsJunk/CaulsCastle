#include "collision_system.h"
#include "bounding_box_component.h"
#include "transform_component.h"
#include "auxiliary.h"
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
                    std::for_each(std::begin(observers), std::end(observers), [&entityA, &entityB, dt](Observer<CollisionEvent>* pObserver)
                    {
                        pObserver->onNotify({entityA, entityB, dt});
                    });
                }
            });
		});
	}
}
