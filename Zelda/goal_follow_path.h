#ifndef TE_GOAL_FOLLOW_PATH_H
#define TE_GOAL_FOLLOW_PATH_H

#include "goal_composite.h"

#include <SFML/Graphics.hpp>

namespace te
{
	class ZeldaEntity;

	class Goal_FollowPath : public Goal<ZeldaEntity>
	{
	public:
		Goal_FollowPath(ZeldaEntity& owner, const std::list<sf::Vector2f>& path);

		void activate();
		Status process(const sf::Time& dt);
		void terminate();
	private:
		ZeldaEntity& mOwner;
		std::list<sf::Vector2f> mPath;
	};
}

#endif
