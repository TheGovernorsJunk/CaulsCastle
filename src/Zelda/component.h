#ifndef TE_COMPONENT_H
#define TE_COMPONENT_H

#include <SFML/Graphics.hpp>

namespace te
{
	class BaseGameEntity;

	class Component
	{
	public:
		virtual ~Component() {}
	};

	class UpdateComponent : public Component
	{
	public:
		virtual ~UpdateComponent() {}
		virtual void update(const sf::Time& dt) = 0;
	};

	class DrawComponent : public Component, public sf::Drawable
	{
	public:
		virtual ~DrawComponent() {}
	};
}

#endif
