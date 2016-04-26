#ifndef TE_GAME_STATE_H
#define TE_GAME_STATE_H

#include <SFML/Graphics.hpp>

namespace te
{
	class StateStack;

	class GameState : public sf::Drawable
	{
	public:
		virtual ~GameState() {}

		virtual bool processInput(const sf::Event& evt) = 0;
		virtual bool update(const sf::Time& dt) = 0;
	protected:
		GameState(StateStack&);

		StateStack& mStateStack;
	};
}

#endif
