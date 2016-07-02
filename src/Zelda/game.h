#ifndef TE_GAME_H
#define TE_GAME_H

#include "runnable.h"
#include "resource_manager.h"
#include "tmx.h"
#include "texture_atlas.h"

#include <SFML/Graphics.hpp>
#include <lua.hpp>

#include <memory>
#include <vector>
#include <functional>

class b2World;

namespace te
{
	class Application;
	class TileMap;
	class EntityManager;
	class MessageDispatcher;
	class BaseGameEntity;
	class TextureManager;

	class Game : public Runnable
	{
	public:
		virtual ~Game();

		//Application& getApplication();
		ResourceManager<sf::Texture>& getTextureManager() { return mTextureManager; }
		const ResourceManager<sf::Texture>& getTextureManager() const { return mTextureManager; }

		EntityManager& getEntityManager() const;
		MessageDispatcher& getMessageDispatcher() const;

		b2World& getPhysicsWorld();
		const b2World& getPhysicsWorld() const;

		void addEntity(std::unique_ptr<BaseGameEntity>&&);

	protected:
		Game(Application& app);

		virtual void update(const sf::Time& dt);
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		ResourceManager<TMX>& getTMXManager() { return mTMXManager; }
		const ResourceManager<TMX>& getTMXManager() const { return mTMXManager; }

		ResourceManager<TextureAtlas>& getAtlasManager() { return mAtlasManager; }
		const ResourceManager<TextureAtlas>& getAtlasManager() const { return mAtlasManager; }

		// For safe resource freeing in ScriptedGame
		void storeLuaState(std::unique_ptr<lua_State, std::function<void(lua_State*)>>&& pL) { mpL = std::move(pL); }

	private:
		Application& mApp;

		std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;

		ResourceManager<TMX> mTMXManager;
		ResourceManager<sf::Texture> mTextureManager;
		ResourceManager<TextureAtlas> mAtlasManager;

		std::unique_ptr<EntityManager> mpEntityManager;
		std::unique_ptr<MessageDispatcher> mpMessageDispatcher;

		std::unique_ptr<b2World> mpWorld;
		std::vector<std::unique_ptr<BaseGameEntity>> mEntities;
	};
}

#endif
