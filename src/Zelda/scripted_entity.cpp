#include "scripted_entity.h"
#include "message_dispatcher.h"
#include "utilities.h"

namespace te
{
	std::unique_ptr<ScriptedEntity> ScriptedEntity::make(Game& world, luabridge::LuaRef entityTable, sf::Vector2f position)
	{
		return std::unique_ptr<ScriptedEntity>{new ScriptedEntity{world, entityTable, position}};
	}

	ScriptedEntity::ScriptedEntity(Game& world, luabridge::LuaRef entityTable, sf::Vector2f position)
		: BaseGameEntity{ world, position }
		, mStateMachines{}
	{
		if (entityTable.isTable() && entityTable["init"].isFunction())
			entityTable["init"](this);
		else
			throw std::runtime_error{"Invalid entity table for ScriptedEntity."};
	}

	void ScriptedEntity::onUpdate(const sf::Time& dt)
	{
		for (auto& fsm : mStateMachines) fsm.update(dt);
	}

	bool ScriptedEntity::handleMessage(const Telegram& msg)
	{
		bool result = false;
		for (auto& fsm : mStateMachines) result = fsm.handleMessage(msg) || result;
		return result;
	}

	void ScriptedEntity::initMachine(luabridge::LuaRef state)
	{
		mStateMachines.push_back({state, *this});
	}
}
