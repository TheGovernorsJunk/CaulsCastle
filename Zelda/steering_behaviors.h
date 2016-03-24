#ifndef TE_STEERING_BEHAVIORS_H
#define TE_STEERING_BEHAVIORS_H

#include <SFML/Graphics.hpp>

namespace te
{
	class Vehicle;
	class BaseGameEntity;
	class Wall2f;

	class SteeringBehaviors
	{
	public:
		enum Deceleration { slow = 3, normal = 2, fast = 1 };

		sf::Vector2f seek(sf::Vector2f target) const;
		sf::Vector2f flee(sf::Vector2f target) const;
		sf::Vector2f arrive(sf::Vector2f target, Deceleration deceleration) const;

		sf::Vector2f pursuit(const Vehicle& target) const;
		sf::Vector2f evade(const Vehicle& target) const;
		sf::Vector2f wander();
		sf::Vector2f obstacleAvoidance(const std::vector<BaseGameEntity&>& obstacles);
		sf::Vector2f wallAvoidance(const std::vector<Wall2f>& walls);
		sf::Vector2f interpose(const Vehicle& agentA, const Vehicle& agentB) const;
		sf::Vector2f hide(const Vehicle& target, std::vector<BaseGameEntity&>& obstacles) const;
		sf::Vector2f followPath();

		sf::Vector2f calculate();
	private:
		sf::Vector2f getHidingPosition(sf::Vector2f posOb, const double radiusOb, sf::Vector2f posTarget) const;
	};
}

#endif
