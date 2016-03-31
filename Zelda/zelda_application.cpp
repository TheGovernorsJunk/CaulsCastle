#include "zelda_application.h"

#include <SFML/Graphics.hpp>

namespace te
{
	ZeldaApplication::ZeldaApplication(const std::string& filename)
		: mTextureManager(std::make_shared<TextureManager>())
		, mGame(mTextureManager)
	{
		mGame.loadMap(filename);
	}

	std::unique_ptr<sf::RenderWindow> ZeldaApplication::makeWindow() const
	{
		return std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 400), "Zelda");
	}

	void ZeldaApplication::processInput(const sf::Event& evt) {}
	void ZeldaApplication::update(const sf::Time& dt) {}
	void ZeldaApplication::render(sf::RenderTarget& target)
	{
		target.draw(mGame);
	}
}
