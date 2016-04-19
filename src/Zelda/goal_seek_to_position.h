#ifndef TE_GOAL_SEEK_TO_POSITION
#define TE_GOAL_SEEK_TO_POSITION

#include "goal.h"

#include <SFML/Graphics.hpp>

namespace te
{
	class ZeldaEntity;

	class Goal_SeekToPosition : public Goal<ZeldaEntity>
	{
	public:
		Goal_SeekToPosition(ZeldaEntity& owner, sf::Vector2f position);

		void activate();
		Status process(const sf::Time& dt);
		void terminate();

	private:
		ZeldaEntity& mOwner;
		sf::Vector2f mPosition;
	};
}

#endif
