#include "zelda_application.h"
#include "zelda_game.h"

#include <SFML/Graphics.hpp>

namespace te
{
	ZeldaApplication::ZeldaApplication(const std::string& filename)
		: mpTextureManager(TextureManager::make())
		, mTextureAtlasMap()
		, mFilename(filename)
	{
		TextureAtlas atlas("textures", "inigo_spritesheet.xml", mpTextureManager.get());
		TextureID id = atlas.getTextureID();
		mTextureAtlasMap.insert({ id, atlas });
	}

	std::unique_ptr<sf::RenderWindow> ZeldaApplication::makeWindow() const
	{
		return std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 400), "Zelda");
	}

	std::unique_ptr<Game> ZeldaApplication::makeGame() const
	{
		auto pGame = ZeldaGame::make(*mpTextureManager, mFilename, 1, 1);
		return pGame;
	}
}
