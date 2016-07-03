#ifndef TE_GAME_H
#define TE_GAME_H

#include "runnable.h"
#include "resource_manager.h"
#include "tmx.h"
#include "texture_atlas.h"
#include "animation.h"
#include "tile_map_layer.h"

#include <SFML/Graphics.hpp>
#include <lua.hpp>

#include <memory>
#include <vector>
#include <functional>

class b2World;

namespace te
{
	class Application;
	class EntityManager;
	class MessageDispatcher;
	class BaseGameEntity;

	class Game : public Runnable
	{
	public:
		virtual ~Game();

		template <typename Resource>
		ResourceID<Resource> load(const std::string& filename)
		{
			return getManager<Resource>().load(filename);
		}
		template <typename Resource>
		ResourceID<Resource> store(std::unique_ptr<Resource>&& pResource)
		{
			return getManager<Resource>().store(std::move(pResource));
		}
		template <typename Resource>
		Resource* get(ResourceID<Resource> id)
		{
			return &getManager<Resource>().get(id);
		}

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

		template <typename T> ResourceManager<T>& getManager();
		template <> ResourceManager<TMX>& getManager() { return mTMXManager; }
		template <> ResourceManager<TextureAtlas>& getManager() { return mAtlasManager; }
		template <> ResourceManager<TileMapLayer>& getManager() { return mLayerManager; }
		template <> ResourceManager<sf::Texture>& getManager() { return mTextureManager; }
		template <> ResourceManager<sf::Sprite>& getManager() { return mSpriteManager; }
		template <> ResourceManager<Animation>& getManager() { return mAnimationManager; }

		std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;

		ResourceManager<TMX> mTMXManager;
		ResourceManager<sf::Texture> mTextureManager;
		ResourceManager<TextureAtlas> mAtlasManager;
		ResourceManager<sf::Sprite> mSpriteManager;
		ResourceManager<Animation> mAnimationManager;
		ResourceManager<TileMapLayer> mLayerManager;

		std::unique_ptr<EntityManager> mpEntityManager;
		std::unique_ptr<MessageDispatcher> mpMessageDispatcher;

		std::unique_ptr<b2World> mpWorld;
		std::vector<std::unique_ptr<BaseGameEntity>> mEntities;
	};
}

#endif
