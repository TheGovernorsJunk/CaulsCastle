#ifndef TE_GOAL_MOVE_TO_POSITION_H
#define TE_GOAL_MOVE_TO_POSITION_H

#include "goal_composite.h"

#include <SFML/Graphics.hpp>

namespace te
{
	class ZeldaEntity;

	class Goal_MoveToPosition : public GoalComposite<ZeldaEntity>
	{
	public:
		Goal_MoveToPosition(ZeldaEntity& owner, sf::Vector2f position);

		void activate();
		Status process(const sf::Time& dt);
		void terminate();

	private:
		ZeldaEntity& mOwner;
		sf::Vector2f mPosition;
	};
}

#endif
