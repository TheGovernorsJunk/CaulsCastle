#ifndef TE_GRAPH_EDGE_H
#define TE_GRAPH_EDGE_H

namespace te
{
	class GraphEdge
	{
	public:
		GraphEdge(int from, int to, double cost = 1.0);
		GraphEdge();
		virtual ~GraphEdge();

		int getFrom() const;
		void setFrom(int index);

		int getTo() const;
		void setTo(int index);

		double getCost() const;
		void setCost(double cost);

	private:
		int mFrom;
		int mTo;
		double mCost;
	};
}

#endif
