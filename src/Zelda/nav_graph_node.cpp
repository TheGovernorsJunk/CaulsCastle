#include "nav_graph_node.h"

namespace te
{
	void NavGraphNode::setPosition(const sf::Vector2f& position)
	{
		mPosition = position;
	}

	sf::Vector2f NavGraphNode::getPosition() const
	{
		return mPosition;
	}
}
