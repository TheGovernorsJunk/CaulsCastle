#ifndef TE_BATTLE_STATE_H
#define TE_BATTLE_STATE_H

#include "game_state.h"
#include "base_game_entity.h"
#include "game.h"
#include "world_state.h"
#include "typedefs.h"
#include "state_machine.h"
#include "state.h"

#include <memory>

namespace te
{
	class BattleGame;
	class SpriteRenderer;
	class Animator;
	class Fighter : public BaseGameEntity
	{
	public:
		enum Command { Attack, Dodge };
		enum class Status { Waiting, Attacking };
		static std::unique_ptr<Fighter> make(BattleGame& world, sf::Vector2f pos = {0, 0});
		bool handleMessage(const Telegram&);
		void onUpdate(const sf::Time&);
		void setFoe(EntityID id) { mFoeID = id; }
		EntityID getFoe() const { return mFoeID; }
		StateMachine<Fighter>& getStateMachine() { return mStateMachine; }
		Animator& getAnimator() { return *mpAnimator; }
	private:
		Fighter(BattleGame& world, sf::Vector2f pos);
		void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
		EntityID mFoeID;
		std::unique_ptr<SpriteRenderer> mpRenderer;
		std::unique_ptr<Animator> mpAnimator;
		StateMachine<Fighter> mStateMachine;
	};

	class WaitState : public State<Fighter>
	{
		void enter(Fighter& entity);
		void execute(Fighter& entity, const sf::Time& dt);
		bool onMessage(Fighter& entity, const Telegram& telegram);
	};

	class AttackState : public State<Fighter>
	{
	public:
		AttackState();
	private:
		void enter(Fighter& entity);
		void execute(Fighter& entity, const sf::Time& dt);
		bool onMessage(Fighter& entity, const Telegram& telegram);
		const sf::Time mDuration;
		const sf::Time mCommitPoint;
		sf::Time mElapsed;
	};

	class BattleGame : public Game
	{
	public:
		static std::unique_ptr<BattleGame> make(Application& app);
		void processInput(const sf::Event&);
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
	private:
		BattleGame(Application& app);
		EntityID mPlayerID;
		EntityID mOpponentID;
	};

	using BattleState = WorldState<true, true, BattleGame>;
}

#endif
