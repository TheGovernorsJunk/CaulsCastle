#ifndef TE_BATTLE_STATE_H
#define TE_BATTLE_STATE_H

#include "game_state.h"
#include "base_game_entity.h"
#include "game.h"
#include "world_state.h"
#include "typedefs.h"
#include "state_machine.h"
#include "state.h"
#include "animator.h"
#include "sprite_renderer.h"

#include <memory>

namespace te
{
	class BattleGame;
	class SpriteRenderer;
	class Animator;
	class Fighter : public BaseGameEntity
	{
	public:
		virtual ~Fighter() {}
		enum Message { Attack, Dodge, ImminentAttack, IncomingAttack };
		static std::unique_ptr<Fighter> make(BattleGame& world, sf::Vector2f pos = {0, 0});
		bool handleMessage(const Telegram&);
		void setFoe(EntityID id) { mFoeID = id; }
		EntityID getFoe() const { return mFoeID; }
		Animator& getAnimator() { return *mpAnimator; }
	protected:
		Fighter(BattleGame& world, sf::Vector2f pos);
		void onUpdate(const sf::Time&);
	private:
		//StateMachine<Fighter>& getStateMachine() { return mStateMachine; }
		void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
		EntityID mFoeID;
		std::unique_ptr<SpriteRenderer> mpRenderer;
		std::unique_ptr<Animator> mpAnimator;
		StateMachine<Fighter> mStateMachine;
	};

	class WaitState : public State<Fighter>
	{
		void enter(Fighter& entity, StateMachine<Fighter>&);
		void execute(Fighter& entity, StateMachine<Fighter>&, const sf::Time& dt);
		bool onMessage(Fighter& entity, StateMachine<Fighter>&, const Telegram& telegram);
	};

	class AttackState : public State<Fighter>
	{
	public:
		AttackState(AnimationID attackAnimation);
	private:
		void enter(Fighter& entity, StateMachine<Fighter>&);
		void execute(Fighter& entity, StateMachine<Fighter>&, const sf::Time& dt);
		bool onMessage(Fighter& entity, StateMachine<Fighter>&, const Telegram& telegram);
		AnimationID mAnimationID;
		sf::Time mDuration;
		sf::Time mDamageStart;
		sf::Time mDamageEnd;
		sf::Time mElapsed;
	};

	class DodgeState : public State<Fighter>
	{
	public:
		DodgeState();
	private:
		void enter(Fighter& entity, StateMachine<Fighter>&);
		void execute(Fighter& entity, StateMachine<Fighter>&, const sf::Time& dt);
		void exit(Fighter& entity, StateMachine<Fighter>&);
		bool onMessage(Fighter& entity, StateMachine<Fighter>&, const Telegram& telegram);
		sf::Time mDuration;
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
