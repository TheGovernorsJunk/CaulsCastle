#include "game_data.h"
#include "utilities.h"

#include <lua.hpp>
#include <LuaBridge.h>

namespace te
{
	static int panic(lua_State* L)
	{
		std::string error{"Invalid or missing parameters in application config file."};
		throw std::runtime_error{error};
		return 0;
	}

	GameData::GameData()
		: config{}
		, physicsWorld{b2Vec2{0, 0}}
		, textureHolder{}
		, atlasHolder{}
		, spriteHolder{}
		, tmxHolder{}
		, mapLayerHolder{}
		, entityIDManager{}
		, positions{}
		, velocities{}
		, sprites{}
		, sortingLayers{}
		, mapLayers{}
		, pendingDraws{}
		, pL{luaL_newstate(), &lua_close}
		, pWindow{}
	{
		lua_State* L = pL.get();
		luaL_openlibs(L);
		doLuaFile(*L, "assets/scripts/config.lua");

		lua_atpanic(L, &panic);
		config.fps = luabridge::getGlobal(L, "FPS");
		config.screenWidth = luabridge::getGlobal(L, "SCREEN_WIDTH");
		config.screenHeight = luabridge::getGlobal(L, "SCREEN_HEIGHT");
		config.windowTitle = luabridge::getGlobal(L, "WINDOW_TITLE").cast<std::string>();
		config.initialScript = luabridge::getGlobal(L, "INITIAL_SCRIPT").cast<std::string>();

		pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode{ config.screenWidth, config.screenHeight }, config.windowTitle);

		pL = { luaL_newstate(), &lua_close };
		luaL_openlibs(pL.get());
	}
}
