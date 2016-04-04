#ifndef TE_GAME_H
#define TE_GAME_H

#include <SFML/Graphics.hpp>

#include <memory>

class b2World;

namespace te
{
	class TileMap;

	class Game : public sf::Transformable, public sf::Drawable
	{
	public:
		Game();
		virtual ~Game();

		bool isPathObstructed(sf::Vector2f a, sf::Vector2f b, float boundingRadius = 0) const;
		const TileMap& getMap() const;
		TileMap& getMap();

		virtual void processInput(const sf::Event& evt) = 0;
		virtual void update(const sf::Time& dt) = 0;

		std::shared_ptr<b2World> getPhysicsWorld();
		std::shared_ptr<const b2World> getPhysicsWorld() const;

	protected:
		void setTileMap(const std::shared_ptr<TileMap>& pTileMap);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		void throwIfNoMap() const;

		std::shared_ptr<TileMap> mpTileMap;
		std::shared_ptr<b2World> mpWorld;
	};
}

#endif
