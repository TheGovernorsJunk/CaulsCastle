#ifndef TE_GAME_H
#define TE_GAME_H

#include "runnable.h"

#include <SFML/Graphics.hpp>
#include <lua.hpp>

#include <memory>
#include <functional>

class b2World;

namespace te
{
	class Application;
	class TileMap;
	class EntityManager;
	class MessageDispatcher;
	class SceneNode;
	class TextureManager;

	class Game : public Runnable
	{
	public:
		virtual ~Game();

		bool isPathObstructed(sf::Vector2f a, sf::Vector2f b, float boundingRadius = 0) const;
		const TileMap& getMap() const;
		TileMap& getMap();

		virtual void update(const sf::Time& dt);

		//Application& getApplication();
		TextureManager& getTextureManager();
		const TextureManager& getTextureManager() const;

		EntityManager& getEntityManager() const;
		MessageDispatcher& getMessageDispatcher() const;

		b2World& getPhysicsWorld();
		const b2World& getPhysicsWorld() const;

		void setPixelToWorldTransform(const sf::Transform&);
		const sf::Transform& getPixelToWorldTransform() const;
		const sf::Transform& getWorldToPixelTransform() const;

	protected:
		Game(Application& app);

		void setTileMap(std::unique_ptr<TileMap>&& pTileMap);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		SceneNode& getSceneGraph();

		// For safe resource freeing in ScriptedGame
		void storeLuaState(std::unique_ptr<lua_State, std::function<void(lua_State*)>>&& pL) { mpL = std::move(pL); }

	private:
		void throwIfNoMap() const;

		Application& mApp;

		std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;
		std::unique_ptr<EntityManager> mpEntityManager;
		std::unique_ptr<MessageDispatcher> mpMessageDispatcher;

		std::unique_ptr<b2World> mpWorld;
		int mTileMapID;
		TileMap* mpTileMap;

		std::unique_ptr<SceneNode> mpSceneGraph;
		sf::Transform mPixelToWorld;
		sf::Transform mWorldToPixel;
	};
}

#endif
