#ifndef TE_GRAPH_SEARCH_DIJKSTRA_H
#define TE_GRAPH_SEARCH_DIJKSTRA_H

#include "indexed_priority_queue.h"

#include <memory>

namespace te
{
	template <class Graph>
	class GraphSearchDijkstra
	{
	public:
		GraphSearchDijkstra(const Graph& graph, int source, int target = -1)
			: mGraph(graph)
			, mShortestPathTree(mGraph.numNodes())
			, mCostToThisNode(mGraph.numNodes())
			, mSearchFrontier(mGraph.numNodes())
			, mSource(source)
			, mTarget(target)
		{
			search();
		}

		std::vector<const typename Graph::Edge*> getAllPaths() const
		{
			return mShortestPathTree;
		}

		std::list<int> getPathToTarget() const
		{
			std::list<int> path;

			if (mTarget < 0 || mTarget >= mGraph.numNodes() || mSearchFrontier[mTarget] == 0) return path;

			int nd = mTarget;

			path.push_back(nd);

			while (nd != mSource)
			{
				nd = mShortestPathTree[nd]->getFrom();
				path.push_back(nd);
			}

			return path;
		}

		double getCostToTarget() const
		{
			return (mTarget < 0 || mTarget >= mGraph.numNodes() || mSearchFrontier[mTarget] == 0) ?
				-1.0 : mCostToThisNode[mTarget];
		}

	private:
		typedef typename Graph::Node Node;
		typedef typename Graph::Edge Edge;

		void search()
		{
			IndexedPriorityQueue<double> pq(mCostToThisNode);
			pq.insert(mSource);

			while (!pq.empty())
			{
				int nextClosestNode = pq.pop();
				mShortestPathTree[nextClosestNode] = mSearchFrontier[nextClosestNode];

				if (nextClosestNode == mTarget) return;

				Graph::ConstEdgeIterator constEdgeIter(mGraph, nextClosestNode);
				for (const Edge* pE = constEdgeIter.begin(); !constEdgeIter.end(); pE = constEdgeIter.next())
				{
					double newCost = mCostToThisNode[nextClosestNode] + pE->getCost();

					// Edge not ever on frontier
					if (mSearchFrontier[pE->getTo()] == 0)
					{
						mCostToThisNode[pE->getTo()] = newCost;
						pq.insert(pE->getTo());
						mSearchFrontier[pE->getTo()] = pE;
					}

					// If cost here is cheaper than on record
					else if ((newCost < mCostToThisNode[pE->getTo()]) && (mShortestPathTree[pE->getTo()] == 0))
					{
						std::vector<size_t> elems = pq.popAll();
						mCostToThisNode[pE->getTo()] = newCost;
						pq.assignElements(std::move(elems));
						mSearchFrontier[pE->getTo()] = pE;
					}
				}
			}
		}

		const Graph& mGraph;
		std::vector<const Edge*> mShortestPathTree;
		std::vector<double> mCostToThisNode;
		std::vector<const Edge*> mSearchFrontier;
		int mSource;
		int mTarget;
	};
}

#endif
