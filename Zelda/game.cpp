#include "game.h"
#include "tile_map.h"

namespace te
{
	Game::~Game() {}

	//bool Game::isPathObstructed(sf::Vector2f a, sf::Vector2f b, float boundingRadius) const
	//{
	//	throwIfNoMap();
	//}

	void Game::setTileMap(const std::shared_ptr<TileMap>& pTileMap)
	{
		if (pTileMap)
		{
			mpTileMap = pTileMap;
		}
		else
		{
			throw std::runtime_error("Must supply TileMap to Game.");
		}
	}

	void Game::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		throwIfNoMap();
		states.transform *= getTransform();
		target.draw(*mpTileMap, states);
	}

	void Game::throwIfNoMap() const
	{
		if (!mpTileMap) throw std::runtime_error("TileMap not set in Game.");
	}
}
