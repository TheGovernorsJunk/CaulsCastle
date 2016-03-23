#ifndef TE_NAV_GRAPH_NODE_H
#define TE_NAV_GRAPH_NODE_H

#include "graph_node.h"

#include <SFML/Graphics.hpp>

namespace te
{
	class NavGraphNode : public GraphNode
	{
	public:
		void setPosition(const sf::Vector2f& position);
		sf::Vector2f getPosition() const;

	private:
		sf::Vector2f mPosition;
	};
}

#endif
