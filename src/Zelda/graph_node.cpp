#include "graph_node.h"

namespace te
{
	GraphNode::GraphNode()
		: mIndex(-1) {}

	GraphNode::GraphNode(int index)
		: mIndex(index) {}

	GraphNode::~GraphNode() {}

	int GraphNode::getIndex() const
	{
		return mIndex;
	}

	void GraphNode::setIndex(int index)
	{
		mIndex = index;
	}
}
