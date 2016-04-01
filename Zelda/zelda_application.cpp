#include "zelda_application.h"
#include "zelda_game.h"

#include <SFML/Graphics.hpp>

namespace te
{
	ZeldaApplication::ZeldaApplication(const std::string& filename)
		: mTextureManager(std::make_shared<TextureManager>())
		, mFilename(filename)
	{
	}

	std::unique_ptr<sf::RenderWindow> ZeldaApplication::makeWindow() const
	{
		return std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 400), "Zelda");
	}

	std::unique_ptr<Game> ZeldaApplication::makeGame() const
	{
		auto pGame = std::make_unique<ZeldaGame>(mTextureManager, mFilename);
		return pGame;
	}
}
