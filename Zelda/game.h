#ifndef TE_GAME_H
#define TE_GAME_H

#include <SFML/Graphics.hpp>

#include <memory>

class b2World;

namespace te
{
	class TileMap;
	class EntityManager;
	class MessageDispatcher;
	class SceneNode;

	class Game : public sf::Transformable, public sf::Drawable
	{
	public:
		virtual ~Game();

		bool isPathObstructed(sf::Vector2f a, sf::Vector2f b, float boundingRadius = 0) const;
		const TileMap& getMap() const;
		TileMap& getMap();

		virtual void processInput(const sf::Event& evt) = 0;
		virtual void update(const sf::Time& dt) = 0;

		EntityManager& getEntityManager();
		MessageDispatcher& getMessageDispatcher();

		b2World& getPhysicsWorld();
		const b2World& getPhysicsWorld() const;

	protected:
		Game();

		void setTileMap(std::unique_ptr<TileMap>&& pTileMap);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		SceneNode& getSceneGraph();

	private:
		void throwIfNoMap() const;

		std::unique_ptr<EntityManager> mpEntityManager;
		std::unique_ptr<MessageDispatcher> mpMessageDispatcher;

		std::unique_ptr<b2World> mpWorld;
		int mTileMapID;
		TileMap* mpTileMap;

		std::unique_ptr<SceneNode> mpSceneGraph;
	};
}

#endif
