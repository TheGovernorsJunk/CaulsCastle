#ifndef TE_GRAPH_NODE_H
#define TE_GRAPH_NODE_H

namespace te
{
	class GraphNode
	{
	public:
		static const int INVALID_INDEX = -1;

		GraphNode();
		GraphNode(int index);
		virtual ~GraphNode();

		int getIndex() const;
		void setIndex(int index);

	private:
		int mIndex;
	};
}

#endif
