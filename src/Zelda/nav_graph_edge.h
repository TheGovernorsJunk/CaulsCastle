#ifndef TE_NAV_GRAPH_EDGE_H
#define TE_NAV_GRAPH_EDGE_H

#include "graph_edge.h"

#include <functional>

namespace te
{
	class NavGraphEdge : public GraphEdge
	{
	public:
		NavGraphEdge(int from, int to, double cost);
	};

}

//constexpr bool std::less<te::NavGraphEdge>::operator()(const te::NavGraphEdge& a, const te::NavGraphEdge& b) const;

#endif
