#include "moving_entity.h"

namespace te
{
	MovingEntity::MovingEntity()
		: BaseGameEntity() {}

	MovingEntity::~MovingEntity() {}

	float MovingEntity::getMaxSpeed() const
	{
		return mMaxSpeed;
	}

	sf::Vector2f MovingEntity::getVelocity() const
	{
		return mVelocity;
	}
}
