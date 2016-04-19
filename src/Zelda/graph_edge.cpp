#include "graph_edge.h"

namespace te
{
	GraphEdge::GraphEdge(int from, int to, double cost)
		: mFrom(from), mTo(to), mCost(cost) {}

	GraphEdge::GraphEdge()
		: mFrom(-1), mTo(-1), mCost(1.0) {}

	GraphEdge::~GraphEdge() {}

	int GraphEdge::getFrom() const
	{
		return mFrom;
	}

	void GraphEdge::setFrom(int index)
	{
		mFrom = index;
	}

	int GraphEdge::getTo() const
	{
		return mTo;
	}

	void GraphEdge::setTo(int index)
	{
		mTo = index;
	}

	double GraphEdge::getCost() const
	{
		return mCost;
	}

	void GraphEdge::setCost(double cost)
	{
		mCost = cost;
	}
}
