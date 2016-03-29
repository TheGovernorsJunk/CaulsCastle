#ifndef TE_SPARSE_GRAPH_H
#define TE_SPARSE_GRAPH_H

#include "nav_graph_node.h"
#include "nav_graph_edge.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <list>
#include <algorithm>

namespace te
{
	template <class NodeType, class EdgeType>
	class SparseGraph : public sf::Drawable
	{
	public:
		typedef NodeType Node;
		typedef EdgeType Edge;

		typedef std::vector<Node> NodeVector;
		typedef std::list<Edge> EdgeList;
		typedef std::vector<EdgeList> EdgeListVector;

		SparseGraph(bool digraph = false)
			: mNodes()
			, mEdges()
			, mbDigraph(digraph)
			, mNextNodeIndex(0)
			, mLineVertices()
		{}

		const Node& getNode(int index) const
		{
			throwIfInvalid(index);
			return mNodes.at(index);
		}
		//Node& getNode(int index)
		//{
		//	return const_cast<Node&>(static_cast<const SparseGraph<Node, Edge>*>(this)->getNode(index));
		//}

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

		//Edge& getEdge(int from, int to)
		//{
		//	return const_cast<Edge&>(static_cast<const SparseGraph<Node, Edge>*>(this)->getEdge(from, to));
		//}

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

		void pruneEdges()
		{
			std::for_each(mEdges.begin(), mEdges.end(), [](EdgeList& edgeList) {
				std::set<Edge> edgeSet(edgeList.begin(), edgeList.end());
				edgeList.assign(edgeSet.begin(), edgeSet.end());
			});
		}

		void prepareVerticesForDrawing() {}

		class ConstEdgeIterator
		{
		public:
			ConstEdgeIterator(const SparseGraph& graph, int node)
				: mGraph(graph), mNode(node), mIter()
			{
				begin();
			}

			const Edge* begin()
			{
				mGraph.throwIfInvalid(mNode);
				mIter = mGraph.mEdges.at(mNode).begin();
				if (!end() && !mGraph.isValid(mIter->getTo())) next();
				return !end() ? &(*mIter) : nullptr;
			}

			const Edge* next()
			{
				mGraph.throwIfInvalid(mNode);
				if (!end())
				{
					do {
						++mIter;
					} while (!end() && !mGraph.isValid(mIter->getTo()));
				}
				return !end() ? &(*mIter) : nullptr;
			}

			bool end()
			{
				mGraph.throwIfInvalid(mNode);
				return mIter == mGraph.mEdges.at(mNode).end();
			}
		private:
			const SparseGraph& mGraph;
			int mNode;
			typename EdgeList::const_iterator mIter;
		};

		class ConstNodeIterator
		{
		public:
			ConstNodeIterator(const SparseGraph& graph)
				: mGraph(graph)
			{
				begin();
			}

			const Node* begin()
			{
				mIter = mGraph.mNodes.begin();
				return !end() ? &(*mIter) : nullptr;
			}

			const Node* next()
			{
				if (!end())
				{
					do {
						++mIter;
					} while (!end() && mIter->getIndex() != Node::INVALID_INDEX);
				}
				return !end() ? &(*mIter) : nullptr;
			}

			bool end()
			{
				return mIter == mGraph.mNodes.end();
			}
		private:
			const SparseGraph& mGraph;
			typename NodeVector::const_iterator mIter;
		};

	private:
		friend class ConstEdgeIterator;

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

		virtual void draw(sf::RenderTarget&, sf::RenderStates) const
		{}

		NodeVector mNodes;
		EdgeListVector mEdges;
		bool mbDigraph;
		int mNextNodeIndex;

		// For use by NavGraph impl
		sf::VertexArray mLineVertices;
	};

	template<> void SparseGraph<NavGraphNode, NavGraphEdge>::prepareVerticesForDrawing()
	{
		mLineVertices.clear();
		mLineVertices.setPrimitiveType(sf::Lines);
		std::for_each(mEdges.begin(), mEdges.end(), [&](const EdgeList& edgeList) {
			std::for_each(edgeList.begin(), edgeList.end(), [&](const Edge& edge) {
				if (isPresent(edge.getFrom()) && isPresent(edge.getTo()))
				{
					mLineVertices.append(sf::Vertex(getNode(edge.getFrom()).getPosition(), sf::Color::Blue));
					mLineVertices.append(sf::Vertex(getNode(edge.getTo()).getPosition(), sf::Color::Blue));
				}
			});
		});
	}

	template<> void SparseGraph<NavGraphNode, NavGraphEdge>::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(mLineVertices, states);
	}
}

#endif
