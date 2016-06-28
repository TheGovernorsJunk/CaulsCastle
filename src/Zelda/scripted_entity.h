#ifndef TE_SCRIPTED_ENTITY_H
#define TE_SCRIPTED_ENTITY_H

#include "base_game_entity.h"
#include "sprite_renderer.h"
#include "animator.h"
#include "scripted_game.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <SFML/Graphics.hpp>

#include <memory>
#include <functional>
#include <vector>

namespace te
{
	class ScriptedGame;

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

		bool handleMessage(const ScriptedGame::ScriptedTelegram& telegram)
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
		static std::unique_ptr<ScriptedEntity> make(ScriptedGame& world, luabridge::LuaRef entityTable, luabridge::LuaRef argsTable, sf::Vector2f position);

		luabridge::LuaRef getUserData() const;
		void initMachine(luabridge::LuaRef);
		void setAnimation(const std::string& anim);
		const std::string& getAnimation() const;
		//void setPositionByTile(int x, int y, EntityID mapID);
		ScriptedGame& getWorld() const { return mWorld; }

	private:
		using FSM = ScriptedStateMachine<ScriptedEntity>;

		ScriptedEntity(ScriptedGame& world, luabridge::LuaRef entityTable, luabridge::LuaRef argsTable, sf::Vector2f position);

		void onUpdate(const sf::Time& dt);
		bool handleMessage(const Telegram& msg);
		void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;

		void initAnimator();

		ScriptedGame& mWorld;
		luabridge::LuaRef mUserData;
		std::vector<FSM> mStateMachines;
		std::unique_ptr<SpriteRenderer> mpSpriteRenderer;
		std::unique_ptr<Animator> mpAnimator;
		std::string mAnimationStr;
	};
}

#endif
