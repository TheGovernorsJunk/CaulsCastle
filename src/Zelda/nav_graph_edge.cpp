#include "nav_graph_edge.h"

namespace te
{
	NavGraphEdge::NavGraphEdge(int from, int to, double cost)
		: GraphEdge(from, to, cost)
	{}
}
