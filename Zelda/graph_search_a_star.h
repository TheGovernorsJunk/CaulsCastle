#ifndef TE_GRAPH_SEARCH_A_STAR_H
#define TE_GRAPH_SEARCH_A_STAR_H

#include "indexed_priority_queue.h"
#include "vector_ops.h"

#include <vector>
#include <list>

namespace te
{
	class HeuristicEuclid
	{
	public:
		HeuristicEuclid(){}

		template<class Graph>
		static double calculate(const Graph& graph, int node1, int node2)
		{
			return distance(graph.getNode(node1).getPosition(), graph.getNode(node2).getPosition());
		}
	};

	template <class Graph, class Heuristic>
	class GraphSearchAStar
	{
	public:
		typedef typename Graph::Edge Edge;

		GraphSearchAStar(const Graph& graph, int source, int target)
			: mGraph(graph)
			, mGCosts(graph.numNodes(), 0.f)
			, mFCosts(graph.numNodes(), 0.f)
			, mShortestPathTree(graph.numNodes(), nullptr)
			, mSearchFrontier(graph.numNodes(), nullptr)
			, mSource(source)
			, mTarget(target)
		{
			search();
		}

		std::vector<const Edge*> getAllPaths() const
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

	private:
		void search()
		{
			IndexedPriorityQueue<double> pq(mFCosts);
			pq.insert(mSource);

			while (!pq.empty())
			{
				int nextClosestNode = pq.pop();
				mShortestPathTree[nextClosestNode] = mSearchFrontier[nextClosestNode];

				if (nextClosestNode == mTarget) return;

				typename Graph::ConstEdgeIterator constEdgeIter(mGraph, nextClosestNode);
				for (const Edge* pEdge = constEdgeIter.begin(); !constEdgeIter.end(); pEdge = constEdgeIter.next())
				{
					double hCost = Heuristic::calculate(mGraph, mTarget, pEdge->getTo());
					double gCost = mGCosts[nextClosestNode] + pEdge->getCost();

					if (mSearchFrontier[pEdge->getTo()] == nullptr)
					{
						mFCosts[pEdge->getTo()] = gCost + hCost;
						mGCosts[pEdge->getTo()] = gCost;
						pq.insert(pEdge->getTo());
						mSearchFrontier[pEdge->getTo()] = pEdge;
					}

					else if ((gCost < mGCosts[pEdge->getTo()]) && (mShortestPathTree[pEdge->getTo()] == nullptr))
					{
						mGCosts[pEdge->getTo()] = gCost;

						std::vector<size_t> elems = pq.popAll();
						mFCosts[pEdge->getTo()] = gCost + hCost;
						pq.assignElements(std::move(elems));

						mSearchFrontier[pEdge->getTo()] = pEdge;
					}
				}
			}
		}

		const Graph& mGraph;
		std::vector<double> mGCosts;
		std::vector<double> mFCosts;
		std::vector<const Edge*> mShortestPathTree;
		std::vector<const Edge*> mSearchFrontier;
		int mSource;
		int mTarget;
	};
}

#endif
