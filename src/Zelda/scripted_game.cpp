#include "scripted_game.h"
#include "utilities.h"

#include <LuaBridge.h>

namespace te
{
	ScriptedGame::ScriptedGame(Application& app, const std::string& initFilename)
		: Game{app}
		, mpL{luaL_newstate(), [](lua_State* L) { lua_close(L); }}
	{
		lua_State* L = mpL.get();

		luaL_openlibs(L);
		doLuaFile(*L, initFilename);
		luabridge::LuaRef init = luabridge::getGlobal(L, "init");

		if (!init.isFunction()) throw std::runtime_error{"Must supply init function."};
		init();
	}

	std::unique_ptr<ScriptedGame> ScriptedGame::make(Application& app, const std::string& initFilename)
	{
		return std::unique_ptr<ScriptedGame>{new ScriptedGame{app, initFilename}};
	}

	void ScriptedGame::processInput(const sf::Event& evt)
	{
	}

	void ScriptedGame::update(const sf::Time&)
	{
	}

	void ScriptedGame::draw(sf::RenderTarget& target, sf::RenderStates) const
	{
		target.clear();
	}
}
