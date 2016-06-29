#ifndef TE_COMPONENT_H
#define TE_COMPONENT_H

namespace sf
{
	class Time;
}

namespace te
{
	class BaseGameEntity;

	class Component
	{
	public:
		virtual void update(const sf::Time& dt) = 0;
	};
}

#endif
