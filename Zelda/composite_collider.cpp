#include "composite_collider.h"

#include <algorithm>

namespace te
{
	void CompositeCollider::addCollider(const BoxCollider& collider)
	{
		mBoxColliders.push_back(collider);
		std::vector<Wall2f> walls = collider.getWalls();
		mWalls.insert(mWalls.end(), walls.begin(), walls.end());
	}

	const std::vector<Wall2f>& CompositeCollider::getWalls() const
	{
		return mWalls;
	}

	//std::vector<Wall2f> CompositeCollider::getWalls() const
	//{
	//	std::vector<Wall2f> walls;
	//	std::for_each(mBoxColliders.begin(), mBoxColliders.end(), [&walls](const BoxCollider& collider) {
	//		std::vector<Wall2f> currWalls = collider.getWalls();
	//		walls.insert(walls.end(), currWalls.begin(), currWalls.end());
	//	});
	//	return walls;
	//}

	bool CompositeCollider::contains(float x, float y) const
	{
		for (auto it = mBoxColliders.begin(); it != mBoxColliders.end(); ++it)
		{
			if (it->contains(x, y)) return true;
		}
		return false;
	}

	void CompositeCollider::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		std::for_each(mBoxColliders.begin(), mBoxColliders.end(), [&target, &states](const BoxCollider& collider) {
			target.draw(collider, states);
		});
	}
}
