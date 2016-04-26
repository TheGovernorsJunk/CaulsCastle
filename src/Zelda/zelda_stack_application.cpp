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
		class OverlayState;
		class ZeldaState : public GameState
		{
		public:
			static std::unique_ptr<ZeldaState> make(Application& app, const std::string& filename)
			{
				return std::unique_ptr<ZeldaState>(new ZeldaState(app, filename));
			}
			bool processInput(const sf::Event& evt)
			{
				mpGame->processInput(evt);
				if (evt.key.code == sf::Keyboard::Space && evt.type == sf::Event::KeyPressed)
					pushState<OverlayState>();
				return true; 
			}
			bool update(const sf::Time& dt)
			{
				mpGame->update(dt);
				return true; 
			}

		private:
			ZeldaState(Application& app, const std::string& filename)
				: GameState()
				, mpGame(ZeldaGame::make(app, app.getTextureManager(), filename, sf::Transform{}.scale(1.f / 16, 1.f / 16)))
			{}
			void draw(sf::RenderTarget& target, sf::RenderStates states) const
			{
				target.draw(*mpGame, states);
			}

			std::unique_ptr<ZeldaGame> mpGame;
		};

		class OverlayState : public GameState
		{
		public:
			static std::unique_ptr<OverlayState> make()
			{
				return std::unique_ptr<OverlayState>(new OverlayState{});
			}
			bool processInput(const sf::Event& evt)
			{
				if (evt.key.code == sf::Keyboard::Escape && evt.type == sf::Event::KeyPressed)
				{
					popState();
					return true;
				}
				else if (evt.key.code == sf::Keyboard::Space)
				{
					return true;
				}
				return false;
			}
			bool update(const sf::Time& dt) { return false; }
		private:
			void draw(sf::RenderTarget& target, sf::RenderStates) const
			{
				sf::Vector2f viewSize = target.getView().getSize();
				sf::RectangleShape rect{viewSize};
				rect.setFillColor(sf::Color{255,0,0,50});
				target.draw(rect);
			}
		};

		auto pStack = StateStack::make<ZeldaState>(*this, mFilename);
		return pStack;
	}
}
