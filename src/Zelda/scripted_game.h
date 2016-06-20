#ifndef TE_SCRIPTED_GAME_H
#define TE_SCRIPTED_GAME_H

#include "game.h"
#include "world_state.h"
#include "typedefs.h"
#include "message_dispatcher.h"

#include <lua.hpp>
#include <LuaBridge.h>

#include <memory>
#include <functional>

namespace te
{
	class Application;
	class ScriptedEntity;
	class CameraEntity;

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
		EntityID loadMap(const std::string& filename);
		TextureID loadSpritesheet(const std::string& filename);
		EntityID makeEntity(luabridge::LuaRef entityTable);
		ScriptedEntity& getScriptedEntity(EntityID id) const;
		CameraEntity& getCamera() const;
		void dispatchMessage(double delay, EntityID sender, EntityID receiver, luabridge::LuaRef info);

		std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;
		luabridge::LuaRef mInputFn;
		CameraEntity* mpCamera;
	};

	using ScriptedState = WorldState<true, true, ScriptedGame, const std::string&>;
}

#endif
