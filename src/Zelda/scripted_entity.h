#ifndef TE_SCRIPTED_ENTITY_H
#define TE_SCRIPTED_ENTITY_H

#include "base_game_entity.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <SFML/Graphics.hpp>

#include <memory>
#include <functional>

namespace te
{
	class Game;

	template <class EntityType>
	class ScriptedStateMachine
	{
	public:
		ScriptedStateMachine(lua_State& L, EntityType& owner)
			: mState{&L}
			, mOwner{owner}
		{}

		void initState(luabridge::LuaRef state)
		{
			verifyState(state);
			mState = state;
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
		static std::unique_ptr<ScriptedEntity> make(Game& world, sf::Vector2f position);

		void loadScript(const std::string& filename);
		ScriptedStateMachine<ScriptedEntity>& getFSM() { return mStateMachine; }

	private:
		using FSM = ScriptedStateMachine<ScriptedEntity>;

		ScriptedEntity(Game& world, sf::Vector2f position);

		void onUpdate(const sf::Time& dt);

		std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;
		FSM mStateMachine;
	};
}

#endif
