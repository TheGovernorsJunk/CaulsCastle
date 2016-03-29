#include"path_planner.h"
#include "moving_entity.h"
#include "game.h"
#include "graph_search_dijkstra.h"
#include "vector_ops.h"

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

		if (closestNodeToTarget = NoClosestNodeFound)
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

	// TODO: Test this implementation
	int PathPlanner::getClosestNodeToPosition(sf::Vector2f pos) const
	{
		if (mNavGraph.numActiveNodes() > 0)
		{
			TileMap::NavGraph::ConstNodeIterator nodeIter(mNavGraph);
			const TileMap::NavGraph::Node* currBest = nodeIter.begin();
			while (!nodeIter.end() && mOwner.getWorld().isPathObstructed(currBest->getPosition(), pos, mOwner.getBoundingRadius()))
			{
				currBest = nodeIter.next();
			}

			if (!nodeIter.end())
			{
				for (const TileMap::NavGraph::Node* pNode = nodeIter.next(); !nodeIter.end(); pNode = nodeIter.next())
				{
					if (distanceSq(pNode->getPosition(), pos) < distanceSq(currBest->getPosition(), pos) && !mOwner.getWorld().isPathObstructed(pos, pNode->getPosition(), mOwner.getBoundingRadius()))
					{
						currBest = pNode;
					}
				}
				return currBest->getIndex();
			}
		}
		return NoClosestNodeFound;
	}

	void PathPlanner::convertIndicesToVectors(const std::list<int> pathOfNodeIndices, std::list<sf::Vector2f>& path)
	{
		for (int index : pathOfNodeIndices)
			path.push_front(mNavGraph.getNode(index).getPosition());
	}
}
