#include "zelda_stack_application.h"
#include "state_stack.h"
#include "game_state.h"
#include "zelda_game.h"

#include <SFML/Graphics.hpp>

namespace te
{
	ZeldaStackApplication::ZeldaStackApplication(const std::string& filename)
		: mFilename(filename)
	{}

	std::unique_ptr<sf::RenderWindow> ZeldaStackApplication::makeWindow() const
	{
		return std::make_unique<sf::RenderWindow>(sf::VideoMode(600, 400), "Zelda Stack");
	}

	std::unique_ptr<Runnable> ZeldaStackApplication::makeRunnable()
	{
		auto pStack = std::make_unique<StateStack>();

		class ZeldaState : public GameState
		{
		public:
			static std::unique_ptr<ZeldaState> make(StateStack& ss, Application& app, const std::string& filename)
			{
				return std::unique_ptr<ZeldaState>(new ZeldaState(ss, app, filename));
			}
			bool processInput(const sf::Event& evt)
			{
				mpGame->processInput(evt);
				return true; 
			}
			bool update(const sf::Time& dt)
			{
				mpGame->update(dt);
				return true; 
			}

		private:
			ZeldaState(StateStack& ss, Application& app, const std::string& filename)
				: GameState(ss)
				, mpGame(ZeldaGame::make(app, app.getTextureManager(), filename, sf::Transform{}.scale(1.f / 16, 1.f / 16)))
			{}
			void draw(sf::RenderTarget& target, sf::RenderStates states) const
			{
				target.draw(*mpGame, states);
			}

			std::unique_ptr<ZeldaGame> mpGame;
		};

		pStack->queuePush<ZeldaState>(*pStack, *this, mFilename);
		return pStack;
	}
}
