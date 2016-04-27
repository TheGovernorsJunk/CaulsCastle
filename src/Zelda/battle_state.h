#ifndef TE_BATTLE_STATE_H
#define TE_BATTLE_STATE_H

#include "game_state.h"

#include <memory>

namespace te
{
	class Fighter {};
	class BattleState : public GameState
	{
	public:
		bool processInput(const sf::Event&);
		bool update(const sf::Time&);
	private:
		friend class StateFactory;
		BattleState(StateStack& ss, Fighter& playerFighter, Fighter& opponent);
		void draw(sf::RenderTarget&, sf::RenderStates) const;

		Fighter& mPlayerFighter;
		Fighter& mOpponent;
		sf::Text mText;
	};
}

#endif
