#ifndef TE_COMPONENT_H
#define TE_COMPONENT_H

#include <SFML/Graphics.hpp>

namespace te
{
	class BaseGameEntity;

	class Component : public sf::Drawable
	{
	public:
		virtual void update(const sf::Time& dt) {}
		virtual void draw(sf::RenderTarget& draw, sf::RenderStates states) const {}
	};
}

#endif
