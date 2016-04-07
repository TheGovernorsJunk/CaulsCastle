#include "moving_entity.h"
#include "vector_ops.h"

namespace te
{
	MovingEntity::MovingEntity(Game& world, float maxSpeed, float maxForce, float maxTurnRate)
		: BaseGameEntity(world, { 0, 0 })
		, mMaxSpeed(maxSpeed)
		, mMaxForce(maxForce)
		, mMaxTurnRate(maxTurnRate)
		, mVelocity(0.f, 0.f)
		, mHeading(0.f, -1.f)
		, mSide(1.f, 0.f)
		, mMass(1.f)
	{}

	MovingEntity::~MovingEntity() {}

	void MovingEntity::updateOnForce(const sf::Time& dt, sf::Vector2f steeringForce)
	{
		float seconds = dt.asSeconds();
		sf::Vector2f acceleration = steeringForce / mMass;
		mVelocity += acceleration * seconds;
		mVelocity = truncate(mVelocity, mMaxSpeed);
		move(mVelocity * seconds);

		if (lengthSq(mVelocity) > 0.00000001f)
		{
			mHeading = normalize(mVelocity);
			mSide = perp(mHeading);
		}
	}

	float MovingEntity::getMaxSpeed() const
	{
		return mMaxSpeed;
	}

	float MovingEntity::getMaxForce() const
	{
		return mMaxForce;
	}

	sf::Vector2f MovingEntity::getVelocity() const
	{
		return mVelocity;
	}
}
