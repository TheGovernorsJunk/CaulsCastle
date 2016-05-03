#ifndef TE_BATTLE_STATE_H
#define TE_BATTLE_STATE_H

#include "game_state.h"
#include "base_game_entity.h"
#include "game.h"
#include "world_state.h"
#include "typedefs.h"

#include <memory>

namespace te
{
	class BattleGame;
	class Fighter : public BaseGameEntity
	{
	public:
		static std::unique_ptr<Fighter> make(BattleGame& world, sf::Vector2f pos = {0, 0});
	private:
		void onDraw(sf::RenderTarget& target, sf::RenderStates states) const;
		Fighter(BattleGame& world, sf::Vector2f pos);
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
