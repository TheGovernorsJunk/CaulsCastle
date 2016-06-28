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

		//Application& getApplication();
		TextureManager& getTextureManager();
		const TextureManager& getTextureManager() const;

		EntityManager& getEntityManager() const;
		MessageDispatcher& getMessageDispatcher() const;

		b2World& getPhysicsWorld();
		const b2World& getPhysicsWorld() const;

	protected:
		Game(Application& app);

		virtual void update(const sf::Time& dt);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		SceneNode& getSceneGraph();

		// For safe resource freeing in ScriptedGame
		void storeLuaState(std::unique_ptr<lua_State, std::function<void(lua_State*)>>&& pL) { mpL = std::move(pL); }

	private:
		Application& mApp;

		std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;

		std::unique_ptr<EntityManager> mpEntityManager;
		std::unique_ptr<MessageDispatcher> mpMessageDispatcher;

		std::unique_ptr<b2World> mpWorld;
		std::unique_ptr<SceneNode> mpSceneGraph;
	};
}

#endif
