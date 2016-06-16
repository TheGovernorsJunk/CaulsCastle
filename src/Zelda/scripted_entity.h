#ifndef TE_SCRIPTED_ENTITY_H
#define TE_SCRIPTED_ENTITY_H

#include "base_game_entity.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <SFML/Graphics.hpp>

#include <memory>
#include <functional>
#include <vector>

namespace te
{
	class Game;

	template <class EntityType>
	class ScriptedStateMachine
	{
	public:
		ScriptedStateMachine(luabridge::LuaRef state, EntityType& owner)
			: mState{state}
			, mOwner{owner}
		{
			verifyState(mState);
		}

		void update(const sf::Time& dt)
		{
			if (!mState.isNil())
			{
				luabridge::LuaRef newState = mState["execute"](&mOwner, dt.asSeconds());
				if (!newState.isNil())
				{
					verifyState(newState);
					if (mState["exit"].isFunction()) mState["exit"](&mOwner);
					mState = newState;
					if (mState["enter"].isFunction()) mState["enter"](&mOwner);
				}
			}
		}

		bool handleMessage(const Telegram& telegram)
		{
			if (!mState.isNil() && mState["onMessage"].isFunction())
			{
				return mState["onMessage"](&mOwner, &telegram);
			}
			return false;
		}

	private:
		static void verifyState(luabridge::LuaRef state)
		{
			if (!state.isTable()) throw std::runtime_error{"Must pass table to changeState."};
			if (!state["execute"].isFunction()) throw std::runtime_error{"Must implement `execute' method."};
		}

		luabridge::LuaRef mState;
		EntityType& mOwner;
	};

	class ScriptedEntity : public BaseGameEntity
	{
	public:
		static std::unique_ptr<ScriptedEntity> make(Game& world, luabridge::LuaRef entityTable, sf::Vector2f position);

		void initMachine(luabridge::LuaRef);

	private:
		using FSM = ScriptedStateMachine<ScriptedEntity>;

		ScriptedEntity(Game& world, luabridge::LuaRef entityTable, sf::Vector2f position);

		void onUpdate(const sf::Time& dt);
		bool handleMessage(const Telegram& msg);

		std::vector<FSM> mStateMachines;
	};
}

#endif
