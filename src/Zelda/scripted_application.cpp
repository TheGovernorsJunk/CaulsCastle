#include "scripted_application.h"
#include "utilities.h"
#include "scripted_game.h"
#include "state_stack.h"

#include <LuaBridge.h>

namespace te
{
	static int panic(lua_State* L)
	{
		std::string error{"Invalid or missing parameters in application config file."};
		throw std::runtime_error{error};
		return 0;
	}

	ScriptedApplication::ScriptedApplication(const std::string& filename)
		: mpL{ luaL_newstate(), [](lua_State* L) { lua_close(L); } }
		, mConfig{}
	{
		lua_State* L = mpL.get();
		luaL_openlibs(L);
		doLuaFile(*L, filename);

		lua_atpanic(L, &panic);
		mConfig.fps = luabridge::getGlobal(L, "FPS");
		mConfig.screenWidth = luabridge::getGlobal(L, "SCREEN_WIDTH");
		mConfig.screenHeight = luabridge::getGlobal(L, "SCREEN_HEIGHT");
		mConfig.windowTitle = luabridge::getGlobal(L, "WINDOW_TITLE").cast<std::string>();
		mConfig.initialScript = luabridge::getGlobal(L, "INITIAL_SCRIPT").cast<std::string>();
	}

	const ScriptedApplication::Config& ScriptedApplication::getConfig() const
	{
		return mConfig;
	}

	std::unique_ptr<sf::RenderWindow> ScriptedApplication::makeWindow() const
	{
		return std::make_unique<sf::RenderWindow>(sf::VideoMode{mConfig.screenWidth, mConfig.screenHeight}, mConfig.windowTitle);
	}

	std::unique_ptr<Runnable> ScriptedApplication::makeRunnable()
	{
		// TODO: This class is redundant and hopefully temporary since it
		// duplicates the ScriptedGame alias for WorldState. Confusing
		// template compilation errors necessitated this short-term solution.
		class ScriptedState : public GameState
		{
		public:
			bool processInput(const sf::Event& evt)
			{
				mpGame->processInput(evt);
				return true; 
			}
			bool update(const sf::Time& dt)
			{
				mpGame->update(dt);
				return true; 
			}

		private:
			friend class StateFactory;
			ScriptedState(StateStack& ss, Application& app, const std::string& filename)
				: GameState(ss)
				, mpGame(ScriptedGame::make(app, filename))
			{}
			void draw(sf::RenderTarget& target, sf::RenderStates states) const
			{
				target.draw(*mpGame, states);
			}

			std::unique_ptr<ScriptedGame> mpGame;
		};

		auto pStack = StateStack::make<ScriptedState>(*this, *this, mConfig.initialScript);
		return pStack;
	}
}
