#ifndef TE_BATTLE_STATE_H
#define TE_BATTLE_STATE_H

#include "game_state.h"
#include "scene_node.h"
#include "game.h"
#include "world_state.h"

#include <memory>

namespace te
{
	class Fighter {};
	class BattleGame : public Game
	{
	public:
		static std::unique_ptr<BattleGame> make(Application& app, Fighter&, Fighter&);
		void processInput(const sf::Event&);
		void update(const sf::Time&);
		void draw(sf::RenderTarget&, sf::RenderStates) const;
	private:
		BattleGame(Application& app, Fighter&, Fighter&);

		Fighter& mPlayerFighter;
		Fighter& mOpponent;
	};

	using BattleState = WorldState<true, true, BattleGame, Fighter, Fighter>;
}

#endif
