#include "moving_entity.h"

namespace te
{
	float MovingEntity::getMaxSpeed() const
	{
		return mMaxSpeed;
	}

	sf::Vector2f MovingEntity::getVelocity() const
	{
		return mVelocity;
	}
}
