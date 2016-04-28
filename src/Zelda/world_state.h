#ifndef TE_WORLD_STATE_H
#define TE_WORLD_STATE_H

#include "game_state.h"

namespace te
{
	template <bool bInput, bool bUpdate, typename GameType, typename... Args>
	class WorldState : public GameState
	{
	public:
		virtual ~WorldState() {}
		bool processInput(const sf::Event& evt)
		{
			mpGame->processInput(evt);
			return bInput;
		}
		bool update(const sf::Time& dt)
		{
			mpGame->update(dt);
			return bUpdate;
		}
	private:
		friend class StateFactory;
		WorldState(StateStack& ss, Args&&... args)
			: GameState(ss)
			, mpGame(GameType::make(getApplication(), std::forward<Args>(args)...))
		{}
		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(*mpGame, states);
		}

		std::unique_ptr<GameType> mpGame;
	};
}

#endif
