#include"path_planner.h"
#include "moving_entity.h"
#include "game.h"
#include "graph_search_dijkstra.h"
#include "vector_ops.h"

namespace te
{
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
			TileMap::NavGraph::ConstNodeIterator currBest;
			auto nodeIter = mNavGraph.nodeBegin();
			do
			{
				currBest = nodeIter;
				++nodeIter;
			} while (nodeIter != mNavGraph.nodeEnd() && (currBest->getIndex() == NavGraphNode::INVALID_INDEX || mOwner.getWorld().isPathObstructed(pos, currBest->getPosition(), mOwner.getBoundingRadius())));

			if (nodeIter != mNavGraph.nodeEnd())
			{
				for (; nodeIter != mNavGraph.nodeEnd(); ++nodeIter)
				{
					if (distanceSq(nodeIter->getPosition(), pos) < distanceSq(currBest->getPosition(), pos) && !mOwner.getWorld().isPathObstructed(pos, nodeIter->getPosition(), mOwner.getBoundingRadius()))
					{
						currBest = nodeIter;
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
