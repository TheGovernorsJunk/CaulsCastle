#ifndef TE_GRAPH_SEARCH_H
#define TE_GRAPH_SEARCH_H

#include <vector>
#include <stack>
#include <memory>

namespace te
{
	template<class Graph>
	class GraphSearchDFS
	{
	public:
		GraphSearchDFS(const std::shared_ptr<Graph>& pGraph, int source, int target = -1)
			: mpGraph(pGraph)
			, mSource(source)
			, mTarget(target)
			, mbFound(false)
			, mVisited(mpGraph->numNodes(), Unvisited)
			, mRoute(mpGraph->numNodes(), NoParentAssigned)
		{
			mbFound = search();
		}

		bool found() const
		{
			return mbFound;
		}

		std::list<int> getPathToTarget() const
		{
			std::list<int> path;

			if (!mbFound || mTarget < 0) return path;

			int nd = mTarget;

			path.push_back(nd);

			while(nd != mSource)
			{
				nd = mRoute[nd];
				path.push_back(nd);
			}

			return path;
		}

	private:
		bool search()
		{
			std::stack<const Edge*> stack;
			Edge start(mSource, mSource, 0);
			stack.push(&start);

			while (!stack.empty())
			{
				const Edge* next = stack.top();
				stack.pop();
				mRoute[next->getTo()] = next->getFrom();
				mVisited[next->getTo()] = Visited;
				if (next->getTo() == mTarget)
				{
					return true;
				}
				Graph::ConstEdgeIterator iter(*mpGraph, next->getTo());
				for (const Edge* pE = iter.begin(); !iter.end(); pE = iter.next())
				{
					if (mVisited[pE->getTo()] == Unvisited)
					{
						stack.push(pE);
					}
				}
			}
			return false;
		}

		enum { Visited, Unvisited, NoParentAssigned };
		typedef typename Graph::Node Node;
		typedef typename Graph::Edge Edge;

		std::shared_ptr<Graph> mpGraph;
		int mSource;
		int mTarget;
		bool mbFound;
		std::vector<int> mVisited;
		std::vector<int> mRoute;
	};
}

#endif
