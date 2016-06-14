#include "scripted_game.h"
#include "utilities.h"

namespace te
{
	ScriptedGame::ScriptedGame(Application& app, const std::string& initFilename)
		: Game{app}
		, mpL{luaL_newstate(), [](lua_State* L) { lua_close(L); }}
	{
		luaL_openlibs(mpL.get());
		doLuaFile(*mpL, initFilename);
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

	void ScriptedGame::draw(sf::RenderTarget&, sf::RenderStates) const
	{}
}
