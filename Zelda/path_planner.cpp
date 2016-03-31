#include"path_planner.h"
#include "moving_entity.h"
#include "game.h"
#include "graph_search_dijkstra.h"
#include "vector_ops.h"

#include <limits>

namespace te
{
	PathPlanner::PathPlanner(MovingEntity& owner)
		: mOwner(owner)
		, mNavGraph(mOwner.getWorld().getMap().getNavGraph())
		, mDestinationPosition(0.f, 0.f)
	{}

	bool PathPlanner::createPathToPosition(sf::Vector2f targetPos, std::list<sf::Vector2f>& path)
	{
		mDestinationPosition = targetPos;

		if (!mOwner.getWorld().isPathObstructed(mOwner.getPosition(), targetPos, mOwner.getBoundingRadius()))
		{
			path.push_back(targetPos);
			return true;
		}

		int closestNode = getClosestNodeToPosition(mOwner.getPosition());

		if (closestNode == NoClosestNodeFound)
		{
			return false;
		}

		int closestNodeToTarget = getClosestNodeToPosition(targetPos);

		if (closestNodeToTarget == NoClosestNodeFound)
		{
			return false;
		}

		// TODO: Use A* instead
		typedef GraphSearchDijkstra<TileMap::NavGraph> Dijkstra;
		Dijkstra search(mNavGraph, closestNode, closestNodeToTarget);

		std::list<int> pathOfNodeIndices = search.getPathToTarget();
		if (!pathOfNodeIndices.empty())
		{
			convertIndicesToVectors(pathOfNodeIndices, path);
			path.push_back(targetPos);
			return true;
		}

		return false;
	}

	int PathPlanner::getClosestNodeToPosition(sf::Vector2f pos) const
	{
		float closestSoFar = std::numeric_limits<float>::max();
		int closestNode = NoClosestNodeFound;

		const float range = mOwner.getWorld().getMap().getCellSpaceNeighborhoodRange();

		TileMap::NavCellSpace& cellSpace = mOwner.getWorld().getMap().getCellSpace();
		cellSpace.calculateNeighbors(pos, range);

		for (const TileMap::NavGraph::Node* pNode = cellSpace.begin(); !cellSpace.end(); pNode = cellSpace.next())
		{
			if (!mOwner.getWorld().isPathObstructed(pNode->getPosition(), pos, mOwner.getBoundingRadius()))
			{
				float dist = distanceSq(pos, pNode->getPosition());
				if (dist < closestSoFar)
				{
					closestSoFar = dist;
					closestNode = pNode->getIndex();
				}
			}
		}

		return closestNode;
	}

	void PathPlanner::convertIndicesToVectors(const std::list<int> pathOfNodeIndices, std::list<sf::Vector2f>& path)
	{
		for (int index : pathOfNodeIndices)
			path.push_front(mNavGraph.getNode(index).getPosition());
	}
}
