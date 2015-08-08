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

	struct CollisionEvent
	{
        Entity a;
        Entity b;
        float dt;
    };

	class CollisionSystem
	{
	public:
        typedef std::vector<Observer<CollisionEvent>*> ObserverList;

        CollisionSystem(
            std::shared_ptr<BoundingBoxComponent> pBoundingBox,
            ObserverList&& observers);

		void update(float dt) const;

    private:
        std::shared_ptr<BoundingBoxComponent> mpBoundingBox;
        ObserverList mObservers;
	};
}

#endif
