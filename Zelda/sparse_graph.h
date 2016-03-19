#ifndef TE_SPARSE_GRAPH_H
#define TE_SPARSE_GRAPH_H

#include <vector>
#include <list>
#include <algorithm>

namespace te
{
	template <class NodeType, class EdgeType>
	class SparseGraph
	{
	public:
		typedef NodeType Node;
		typedef EdgeType Edge;

		typedef std::vector<Node> NodeVector;
		typedef std::list<Edge> EdgeList;
		typedef std::vector<EdgeList> EdgeListVector;

		SparseGraph(bool digraph = false)
			: mNextNodeIndex(0)
			, mbDigraph(digraph)
		{}

		const Node& getNode(int index) const
		{
			throwIfInvalid(index);
			return mNodes.at(index);
		}
		Node& getNode(int index)
		{
			return const_cast<Node&>(static_cast<const SparseGraph<Node, Edge>*>(this)->getNode(index));
		}

		const Edge& getEdge(int from, int to) const
		{
			throwIfInvalid(from);
			throwIfInvalid(to);

			const std::list<Edge>& edgeList = mEdges.at(from);
			const Edge* retEdge = nullptr;
			std::for_each(edgeList.begin(), edgeList.end(), [to, &retEdge](const Edge& edge) {
				if (edge.getTo() == to) {
					retEdge = &edge;
				}
			});
			if (retEdge)
			{
				return *retEdge;
			}
			else
			{
				throw std::runtime_error("No edge with given `from' and `to' nodes.");
			}
		}

		Edge& getEdge(int from, int to)
		{
			return const_cast<Edge&>(static_cast<const SparseGraph<Node, Edge>*>(this)->getEdge(from, to));
		}

		int getNextFreeNodeIndex() const
		{
			return mNextNodeIndex;
		}

		int addNode(Node node)
		{
			node.setIndex(mNextNodeIndex++);
			mNodes.push_back(node);
			mEdges.push_back(EdgeList());
			return node.getIndex();
		}

		void removeNode(int node)
		{
			throwIfInvalid(node);
			mNodes.at(node).setIndex(Node::INVALID_INDEX);
		}

		void addEdge(const Edge& edge)
		{
			throwIfInvalid(edge.getFrom());
			throwIfInvalid(edge.getTo());

			mEdges.at(edge.getFrom()).push_front(edge);
			// If it's not a digraph, add edge for other direction
			if (!mbDigraph)
			{
				Edge reverseEdge = edge;
				reverseEdge.setFrom(edge.getTo());
				reverseEdge.setTo(edge.getFrom());

				mEdges.at(reverseEdge.getFrom()).push_front(std::move(reverseEdge));
			}
		}

		void removeEdge(int from, int to)
		{
			throwIfInvalid(from);
			throwIfInvalid(to);

			uniRemoveEdge(from, to);

			if (!mbDigraph)
			{
				uniRemoveEdge(to, from);
			}
		}

		int numNodes() const
		{
			return mNodes.size();
		}

		int numActiveNodes() const
		{
			int num = 0;
			std::for_each(mNodes.begin(), mNodes.end(), [&num](const Node& node) {
				if (node.getIndex() != Node::INVALID_INDEX)
					++num;
			});
			return num;
		}

		int numEdges() const
		{
			int num = 0;
			for (auto it = mEdges.begin(); it != mEdges.end(); ++it) {
				if (isValid(it - mEdges.begin())) {
					std::for_each(it->begin(), it->end(), [this, &num](const Edge& edge) {
						if (isValid(edge.getTo())) {
							++num;
						}
					});
				}
			}
			return mbDigraph ? num : num / 2;
		}

		bool isDigraph() const
		{
			return mbDigraph;
		}

		bool isEmpty() const
		{
			return mNodes.size() == 0;
		}

		bool isPresent(int index) const
		{
			return isValid(index);
		}

		void clear()
		{
			mNodes.clear();
			mEdges.clear();
		}

	private:
		bool isValid(int index) const
		{
			return index >= 0 && (unsigned)index < mNodes.size() && mNodes.at(index).getIndex() != Node::INVALID_INDEX;
		}
		void throwIfInvalid(int index) const
		{
			if (!isValid(index))
				throw std::runtime_error("Given node index is invalid.");
		}
		void uniRemoveEdge(int from, int to)
		{
			EdgeList& edgeList = mEdges.at(from);
			auto newEnd = std::remove_if(edgeList.begin(), edgeList.end(), [from, to](const Edge& edge) {
				return edge.getFrom() == from && edge.getTo() == to;
			});
			if (newEnd == edgeList.end())
			{
				throw std::runtime_error("No edge exists for given `to' and `from' indices.");
			}
			edgeList.erase(newEnd, edgeList.end());
		}

		NodeVector mNodes;
		EdgeListVector mEdges;
		bool mbDigraph;
		int mNextNodeIndex;
	};
}

#endif
