#ifndef TE_SCRIPTED_GAME_H
#define TE_SCRIPTED_GAME_H

#include "game.h"
#include "world_state.h"

#include <lua.hpp>

#include <memory>
#include <functional>

namespace te
{
	class Application;

	class ScriptedGame : public Game
	{
	public:
		static std::unique_ptr<ScriptedGame> make(Application& app, const std::string& initFilename);
		void processInput(const sf::Event&);
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
	private:
		ScriptedGame(Application& app, const std::string& initFilename);

		std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;
	};

	using ScriptedState = WorldState<true, true, ScriptedGame, const std::string&>;
}

#endif
