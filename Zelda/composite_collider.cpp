#include "composite_collider.h"
#include "wall.h"

#include <algorithm>

namespace te
{
	void CompositeCollider::addCollider(const BoxCollider& collider)
	{
		mBoxColliders.push_back(collider);
	}

	std::vector<Wall2f> CompositeCollider::getWalls() const
	{
		std::vector<Wall2f> walls;
		std::for_each(mBoxColliders.begin(), mBoxColliders.end(), [&walls](const BoxCollider& collider) {
			std::vector<Wall2f> currWalls = collider.getWalls();
			walls.insert(walls.end(), currWalls.begin(), currWalls.end());
		});
		return walls;
	}

	void CompositeCollider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		std::for_each(mBoxColliders.begin(), mBoxColliders.end(), [&target, &states](const BoxCollider& collider) {
			target.draw(collider, states);
		});
	}
}
