#include "zelda_game.h"
#include "tmx.h"
#include "tile_map.h"

namespace te
{
	ZeldaGame::ZeldaGame(const std::shared_ptr<TextureManager>& pTextureManager)
		: mpTextureManager(pTextureManager)
	{
		if (!pTextureManager)
		{
			throw std::runtime_error("Must supply TextureManager in ZeldaGame ctor.");
		}
	}

	void ZeldaGame::loadMap(const std::string& fileName)
	{
		setTileMap(std::make_shared<TileMap>(*mpTextureManager, TMX(fileName)));
	}

	void ZeldaGame::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		Game::draw(target, states);
	}
}
