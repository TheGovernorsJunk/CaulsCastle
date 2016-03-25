#include "steering_behaviors.h"
#include "vector_ops.h"
#include "vehicle.h"

namespace te
{
	SteeringBehaviors::SteeringBehaviors(Owner& vehicle)
		: mOwner(vehicle)
		, mSteeringForce(0.f, 0.f)
		, mSeekEnabled(false)
		, mSeekTarget()
		, mFleeEnabled(false)
		, mFleeTarget()
		, mPanicDistance(0.f)
		, mArriveEnabled(false)
		, mArriveTarget()
		, mDeceleration(Deceleration::Normal)
	{}

	sf::Vector2f SteeringBehaviors::calculate()
	{
		mSteeringForce = sf::Vector2f(0.f, 0.f);

		sf::Vector2f force;
		if (mSeekEnabled)
		{
			force = seek(mSeekTarget);
			if (!accumulateForce(mSteeringForce, force)) return mSteeringForce;
		}
		if (mFleeEnabled)
		{
			force = flee(mFleeTarget, mPanicDistance);
			if (!accumulateForce(mSteeringForce, force)) return mSteeringForce;
		}
		if (mArriveEnabled)
		{
			force = arrive(mArriveTarget, mDeceleration);
			if (!accumulateForce(mSteeringForce, force)) return mSteeringForce;
		}

		return mSteeringForce;
	}

	void SteeringBehaviors::setSeekEnabled(bool enabled, sf::Vector2f target)
	{
		mSeekEnabled = enabled;
		mSeekTarget = target;
	}

	void SteeringBehaviors::setFleeEnabled(bool enabled, sf::Vector2f target, float panicDistance)
	{
		mFleeEnabled = enabled;
		mFleeTarget = target;
		mPanicDistance = panicDistance;
	}

	void SteeringBehaviors::setArriveEnabled(bool enabled, sf::Vector2f target, SteeringBehaviors::Deceleration deceleration)
	{
		mArriveEnabled = enabled;
		mArriveTarget = target;
		mDeceleration = deceleration;
	}

	sf::Vector2f SteeringBehaviors::seek(sf::Vector2f target) const
	{
		sf::Vector2f desiredVelocity = normalize(target - mOwner.getPosition()) * mOwner.getMaxSpeed();
		return (desiredVelocity - mOwner.getVelocity());
	}

	sf::Vector2f SteeringBehaviors::flee(sf::Vector2f target, float panicDistance) const
	{
		if (panicDistance > 0.f)
		{
			const double panicDistanceSq = panicDistance * panicDistance;
			if (distanceSq(mOwner.getPosition(), target) > panicDistanceSq)
			{
				return sf::Vector2f(0.f, 0.f);
			}
		}

		sf::Vector2f desiredVelocity = normalize(mOwner.getPosition() - target) * mOwner.getMaxSpeed();
		return (desiredVelocity - mOwner.getVelocity());
	}

	sf::Vector2f SteeringBehaviors::arrive(sf::Vector2f target, SteeringBehaviors::Deceleration deceleration) const
	{
		sf::Vector2f toTarget = target - mOwner.getPosition();
		float dist = length(toTarget);

		if (dist > 0)
		{
			// TODO: Find better tweaker
			const float decelerationTweaker = 0.3f;
			float speed = dist / ((float)deceleration * decelerationTweaker);
			speed = std::min(speed, mOwner.getMaxSpeed());
			sf::Vector2f desiredVelocity = toTarget * speed / dist;
			return (desiredVelocity - mOwner.getVelocity());
		}

		return sf::Vector2f(0.f, 0.f);
	}

	bool SteeringBehaviors::accumulateForce(sf::Vector2f& accumulator, sf::Vector2f force) const
	{
		float magnitude = length(accumulator);
		float magnitudeRemaining = mOwner.getMaxForce() - magnitude;
		if (magnitudeRemaining <= 0.f) return false;
		float magnitudeToAdd = length(force);
		if (magnitudeToAdd < magnitudeRemaining)
		{
			accumulator += force;
		}
		else
		{
			accumulator += normalize(force) * magnitudeRemaining;
		}
		return true;
	}
}
