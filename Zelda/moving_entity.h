#ifndef TE_MOVING_ENTITY_H
#define TE_MOVING_ENTITY_H

#include "base_game_entity.h"

namespace te
{
	class MovingEntity : public BaseGameEntity
	{
	public:
		MovingEntity(const std::shared_ptr<Game>& pWorld, float maxSpeed = 0.f, float maxForce = 0.f, float maxTurnRate = 0.f);
		virtual ~MovingEntity();

		float getMaxSpeed() const;
		float getMaxForce() const;
		sf::Vector2f getVelocity() const;

		void updateOnForce(const sf::Time& dt, sf::Vector2f steeringForce);

	private:
		float mMaxSpeed;
		float mMaxForce;
		float mMaxTurnRate;
		sf::Vector2f mVelocity;
		sf::Vector2f mHeading;
		sf::Vector2f mSide;
		float mMass;
	};
}

#endif
