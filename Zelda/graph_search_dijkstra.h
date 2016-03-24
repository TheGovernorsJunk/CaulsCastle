#ifndef TE_GRAPH_SEARCH_DIJKSTRA_H
#define TE_GRAPH_SEARCH_DIJKSTRA_H

#include <queue>
#include <memory>

namespace te
{
	template <class T>
	class IndexedPriorityQueue
	{
	public:
		IndexedPriorityQueue(const std::vector<T>& keys, size_t size)
			: mCompare([&keys](size_t lhs, size_t rhs) { return keys.at(lhs) > keys.at(rhs); })
			, mPriorityQueue(mCompare)
		{
		}

		void insert(size_t index)
		{
			mPriorityQueue.push(index);
		}

		bool empty() const
		{
			return mPriorityQueue.empty();
		}

		size_t pop()
		{
			size_t index = mPriorityQueue.top();
			mPriorityQueue.pop();
			return index;
		}

		void resort()
		{
			mPriorityQueue.push(pop());
		}

	private:
		std::function<bool(size_t, size_t)> mCompare;
		std::priority_queue<size_t, std::vector<size_t>, std::function<bool(size_t, size_t)>> mPriorityQueue;
	};

	template <class Graph>
	class GraphSearchDijkstra
	{
	public:
		GraphSearchDijkstra(const std::shared_ptr<Graph>& pGraph, int source, int target = -1)
			: mpGraph(pGraph)
			, mShortestPathTree(pGraph->numNodes())
			, mCostToThisNode(pGraph->numNodes())
			, mSearchFrontier(pGraph->numNodes())
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

			if (mTarget < 0 || mTarget >= mpGraph->numNodes() || mSearchFrontier[mTarget] == 0) return path;

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
			return (mTarget < 0 || mTarget >= mpGraph->numNodes() || mSearchFrontier[mTarget] == 0) ?
				-1.0 : mCostToThisNode[mTarget];
		}

	private:
		typedef typename Graph::Node Node;
		typedef typename Graph::Edge Edge;

		void search()
		{
			IndexedPriorityQueue<double> pq(mCostToThisNode, mpGraph->numNodes());
			pq.insert(mSource);

			while (!pq.empty())
			{
				int nextClosestNode = pq.pop();
				mShortestPathTree[nextClosestNode] = mSearchFrontier[nextClosestNode];

				if (nextClosestNode == mTarget) return;

				Graph::ConstEdgeIterator constEdgeIter(*mpGraph, nextClosestNode);
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
						mCostToThisNode[pE->getTo()] = newCost;
						pq.resort();
						mSearchFrontier[pE->getTo()] = pE;
					}
				}
			}
		}

		std::shared_ptr<Graph> mpGraph;
		std::vector<const Edge*> mShortestPathTree;
		std::vector<double> mCostToThisNode;
		std::vector<const Edge*> mSearchFrontier;
		int mSource;
		int mTarget;
	};
}

#endif
