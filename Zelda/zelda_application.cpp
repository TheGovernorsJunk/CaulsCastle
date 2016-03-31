#include "zelda_application.h"

#include <SFML/Graphics.hpp>

namespace te
{
	std::unique_ptr<sf::RenderWindow> ZeldaApplication::makeWindow() const
	{
		return std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 400), "Zelda");
	}

	void ZeldaApplication::processInput(const sf::Event& evt) {}
	void ZeldaApplication::update(const sf::Time& dt) {}
	void ZeldaApplication::render(sf::RenderTarget& target) {}
}
