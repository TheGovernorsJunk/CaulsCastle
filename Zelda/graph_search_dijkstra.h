#ifndef TE_GRAPH_SEARCH_DIJKSTRA_H
#define TE_GRAPH_SEARCH_DIJKSTRA_H

#include <memory>

namespace te
{
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
		{}

	private:
		typedef typename Graph::Edge Edge;
		typedef typename Graph::Node Node;

		std::shared_ptr<Graph> mpGraph;
		std::vector<const Edge*> mShortestPathTree;
		std::vector<double> mCostToThisNode;
		std::vector<const Edge*> mSearchFrontier;
		int mSource;
		int mTarget;
	};
}

#endif
