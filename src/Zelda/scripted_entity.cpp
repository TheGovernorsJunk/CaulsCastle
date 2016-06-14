#include "scripted_entity.h"

namespace te
{
	std::unique_ptr<ScriptedEntity> ScriptedEntity::make(Game& world, sf::Vector2f position)
	{
		return std::unique_ptr<ScriptedEntity>{new ScriptedEntity{world, position}};
	}

	void ScriptedEntity::loadScript(const std::string& filename)
	{
		int status = luaL_dofile(mpL.get(), filename.c_str());
		if (status)
		{
			throw std::runtime_error("Could not load Lua script.");
		}

		luabridge::LuaRef main = luabridge::getGlobal(mpL.get(), "main");
		if (!main.isFunction()) throw std::runtime_error("Must provide main function.");
		main(luabridge::LuaRef{mpL.get(), this});
	}

	ScriptedEntity::ScriptedEntity(Game& world, sf::Vector2f position)
		: BaseGameEntity{world, position}
		, mpL{luaL_newstate(), [](lua_State* pL) { lua_close(pL); }}
		, mStateMachine{*mpL, *this}
	{
		lua_State* L = mpL.get();
		luaL_openlibs(L);
		luabridge::getGlobalNamespace(L)
			.beginClass<ScriptedEntity>("ScriptedEntity")
				.addFunction("getFSM", &ScriptedEntity::getFSM)
			.endClass()
			.beginClass<FSM>("ScriptedStateMachine")
				.addFunction("changeState", &FSM::changeState)
			.endClass();
	}

	void ScriptedEntity::onUpdate(const sf::Time& dt)
	{
		mStateMachine.update(dt);
	}
}
