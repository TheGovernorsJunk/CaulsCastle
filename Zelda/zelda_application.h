#ifndef TE_ZELDA_APPLICATION_H
#define TE_ZELDA_APPLICATION_H

#include "application.h"

namespace te
{
	class ZeldaApplication : public Application
	{
	private:
		std::unique_ptr<sf::RenderWindow> makeWindow() const;
		void processInput(const sf::Event& evt);
		void update(const sf::Time& dt);
		void render(sf::RenderTarget& target);
	};
}

#endif
