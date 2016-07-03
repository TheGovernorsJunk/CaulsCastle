#ifndef TE_SCRIPTED_GAME_H
#define TE_SCRIPTED_GAME_H

#include "game.h"
#include "world_state.h"
#include "typedefs.h"
#include "message_dispatcher.h"
#include "animation.h"

#include <lua.hpp>
#include <LuaBridge.h>

#include <memory>
#include <functional>

namespace te
{
	class Application;
	class ScriptedEntity;
	class CameraEntity;
	struct RayCastHit;
	class AABB;

	class ScriptedGame : public Game
	{
	public:
		struct ScriptedInfo : public Telegram::Info
		{
			luabridge::LuaRef ref;
			ScriptedInfo(luabridge::LuaRef r) : ref(r) {}
		};
		struct ScriptedTelegram
		{
			double dispatchTime;
			int sender;
			int receiver;
			luabridge::LuaRef info;
		};

		~ScriptedGame();
		static std::unique_ptr<ScriptedGame> make(Application& app, const std::string& initFilename);
		void processInput(const sf::Event&);
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
	private:
		ScriptedGame(Application& app, const std::string& initFilename);
		luabridge::LuaRef makeMapLayers(ResourceID<TMX>);
		luabridge::LuaRef getAtlasSprites(ResourceID<TextureAtlas>) const;
		ResourceID<sf::Sprite> makeSprite(ResourceID<sf::Texture>, luabridge::LuaRef rect);
		ResourceID<Animation> makeAnimation(luabridge::LuaRef spriteArr, int millisecondsPerFrame);
		EntityID makeEntity(luabridge::LuaRef entityTable, luabridge::LuaRef argsTable);
		ScriptedEntity* getScriptedEntity(EntityID id) const;
		CameraEntity& getCamera() const;
		void dispatchMessage(double delay, EntityID sender, EntityID receiver, luabridge::LuaRef info);
		TileMap* getMap(EntityID mapID) const;
		luabridge::LuaRef getObjects(ResourceID<TMX> tmxID, const std::string& groupName) const;
		luabridge::LuaRef getLayerNames(ResourceID<TMX> tmxID) const;
		float getAnimationDuration(const std::string& animationStr) const;
		bool rayCast(sf::Vector2f origin, sf::Vector2f direction, RayCastHit* pHitInfo, float maxDistance = std::numeric_limits<float>::max() * 0.5f);
		luabridge::LuaRef getEntitiesInRegion(const AABB*) const;

		std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;
		luabridge::LuaRef mKeyInputFn;
		luabridge::LuaRef mMouseButtonInputFn;
		luabridge::LuaRef mAxisInputFn;
		luabridge::LuaRef mUpdateFn;
		CameraEntity* mpCamera;
	};

	using ScriptedState = WorldState<true, true, ScriptedGame, const std::string&>;
}

#endif
