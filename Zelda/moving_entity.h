#ifndef TE_MOVING_ENTITY_H
#define TE_MOVING_ENTITY_H

#include "base_game_entity.h"

namespace te
{
	class MovingEntity : public BaseGameEntity
	{
	public:
		float getMaxSpeed() const;
		sf::Vector2f getVelocity() const;
	private:
		float mMaxSpeed;
		sf::Vector2f mVelocity;
	};
}

#endif
