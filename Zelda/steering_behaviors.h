#ifndef TE_STEERING_BEHAVIORS_H
#define TE_STEERING_BEHAVIORS_H

#include <SFML/Graphics.hpp>

namespace te
{
	class MovingEntity;
	class BaseGameEntity;
	class Wall2f;

	class SteeringBehaviors
	{
	public:
		typedef MovingEntity TargetEntity;
		typedef MovingEntity Owner;

		SteeringBehaviors(Owner& owner);

		enum class Deceleration { slow = 3, normal = 2, fast = 1 };

		sf::Vector2f calculate();

		void setSeekEnabled(bool enabled, sf::Vector2f target = sf::Vector2f(0.f, 0.f));

	private:
		sf::Vector2f seek(sf::Vector2f target) const;
		sf::Vector2f flee(sf::Vector2f target, float panicDistance = 0.f) const;
		sf::Vector2f arrive(sf::Vector2f target, Deceleration deceleration) const;

		sf::Vector2f pursuit(const TargetEntity& target) const;
		sf::Vector2f evade(const TargetEntity& target) const;
		sf::Vector2f wander();
		sf::Vector2f obstacleAvoidance(const std::vector<BaseGameEntity&>& obstacles);
		sf::Vector2f wallAvoidance(const std::vector<Wall2f>& walls);
		sf::Vector2f interpose(const TargetEntity& agentA, const TargetEntity& agentB) const;
		sf::Vector2f hide(const TargetEntity& target, std::vector<BaseGameEntity&>& obstacles) const;
		sf::Vector2f followPath();

		sf::Vector2f getHidingPosition(sf::Vector2f posOb, const double radiusOb, sf::Vector2f posTarget) const;

		bool accumulateForce(sf::Vector2f& accumulator, sf::Vector2f force) const;

		Owner& mOwner;
		sf::Vector2f mSteeringForce;

		bool mSeekEnabled;
		sf::Vector2f mSeekTarget;
	};
}

#endif
