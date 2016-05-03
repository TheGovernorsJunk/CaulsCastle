#include "zelda_stack_application.h"
#include "state_stack.h"
#include "game_state.h"
#include "zelda_game.h"
#include "battle_state.h"

#include <SFML/Graphics.hpp>

namespace te
{
	ZeldaStackApplication::ZeldaStackApplication(const std::string& filename)
		: mFilename(filename)
	{}

	std::unique_ptr<sf::RenderWindow> ZeldaStackApplication::makeWindow() const
	{
		return std::make_unique<sf::RenderWindow>(sf::VideoMode(1600, 900), "Zelda Stack");
	}

	std::unique_ptr<Runnable> ZeldaStackApplication::makeRunnable()
	{
		class ZeldaState : public GameState
		{
		public:
			bool processInput(const sf::Event& evt)
			{
				mpGame->processInput(evt);
				if (evt.key.code == sf::Keyboard::Space && evt.type == sf::Event::KeyPressed)
					pushState<BattleState>();
				return true; 
			}
			bool update(const sf::Time& dt)
			{
				mpGame->update(dt);
				return true; 
			}

		private:
			friend class StateFactory;
			ZeldaState(StateStack& ss, Application& app, const std::string& filename)
				: GameState(ss)
				, mpGame(ZeldaGame::make(app, filename, sf::Transform{}.scale(1.f / 16, 1.f / 16)))
			{}
			void draw(sf::RenderTarget& target, sf::RenderStates states) const
			{
				target.draw(*mpGame, states);
			}

			std::unique_ptr<ZeldaGame> mpGame;
		};

		auto pStack = StateStack::make<ZeldaState>(*this, *this, mFilename);
		return pStack;
	}
}
