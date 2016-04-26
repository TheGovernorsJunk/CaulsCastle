#ifndef TE_RUNNABLE_H
#define TE_RUNNABLE_H

#include <SFML/Graphics.hpp>

namespace te
{
	class Runnable : public sf::Drawable
	{
	public:
		virtual void processInput(const sf::Event&) = 0;
		virtual void update(const sf::Time&) = 0;
	};
}

#endif
