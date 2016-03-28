#ifndef TE_PATH_PLANNER_H
#define TE_PATH_PLANNER_H

#include "tile_map.h"

#include <SFML/Graphics.hpp>

#include <list>

namespace te
{
	class MovingEntity;

	class PathPlanner
	{
	public:
		PathPlanner(MovingEntity& owner);
		bool createPathToPosition(sf::Vector2f targetPosition, std::list<sf::Vector2f>& path);

	private:
		enum { NoClosestNodeFound = -1 };

		int getClosestNodeToPosition(sf::Vector2f pos) const;
		void convertIndicesToVectors(const std::list<int> pathOfNodeIndices, std::list<sf::Vector2f>& path);

		MovingEntity& mOwner;
		const TileMap::NavGraph& mNavGraph;
		sf::Vector2f mDestinationPosition;
	};
}

#endif
