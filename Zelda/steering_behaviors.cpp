#include "steering_behaviors.h"
#include "vector_ops.h"
#include "vehicle.h"

namespace te
{
	SteeringBehaviors::SteeringBehaviors(Vehicle& vehicle)
		: mOwner(vehicle) {}

	sf::Vector2f SteeringBehaviors::seek(sf::Vector2f target) const
	{
		sf::Vector2f desiredVelocity = normalize(target - mOwner.getPosition()) * mOwner.getMaxSpeed();
		return (desiredVelocity - mOwner.getVelocity());
	}

	sf::Vector2f SteeringBehaviors::flee(sf::Vector2f target, float panicDistance) const
	{
		if (panicDistance > 0.f)
		{
			const double panicDistanceSq = 100.0 * 100.0;
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
			const float decelerationTweaker = 0.3f;
			float speed = dist / ((float)deceleration * decelerationTweaker);
			speed = std::min(speed, mOwner.getMaxSpeed());
			sf::Vector2f desiredVelocity = toTarget * speed / dist;
			return (desiredVelocity - mOwner.getVelocity());
		}

		return sf::Vector2f(0.f, 0.f);
	}
}
