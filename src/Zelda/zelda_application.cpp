#include "zelda_application.h"
#include "zelda_game.h"

#include <SFML/Graphics.hpp>

namespace te
{
	ZeldaApplication::ZeldaApplication(const std::string& filename)
		: mFilename(filename)
	{}

	std::unique_ptr<sf::RenderWindow> ZeldaApplication::makeWindow() const
	{
		return std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 400), "Zelda");
	}

	std::unique_ptr<Game> ZeldaApplication::makeGame()
	{
		sf::Transform transform;
		transform.scale(1.f / 16, 1.f / 16);
		auto pGame = ZeldaGame::make(*this, getTextureManager(), mFilename, transform);
		return pGame;
	}
}
