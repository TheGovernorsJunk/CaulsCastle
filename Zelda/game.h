#ifndef TE_GAME_H
#define TE_GAME_H

#include <SFML/Graphics.hpp>

#include <memory>

namespace te
{
	class TileMap;

	class Game : public sf::Transformable, public sf::Drawable
	{
	public:
		virtual ~Game();

		bool isPathObstructed(sf::Vector2f a, sf::Vector2f b, float boundingRadius = 0) const;

	protected:
		void setTileMap(const std::shared_ptr<TileMap>& pTileMap);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		void throwIfNoMap() const;

		std::shared_ptr<TileMap> mpTileMap;
	};
}

#endif
