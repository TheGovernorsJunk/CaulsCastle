#ifndef TE_APPLICATION_H
#define TE_APPLICATION_H

#include <memory>

namespace sf
{
	class Time;
	class Event;
	class RenderWindow;
	class RenderTarget;
}

namespace te
{
	class Game;

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run(int fps = 60);
	private:
		virtual std::unique_ptr<sf::RenderWindow> makeWindow() const = 0;
		virtual std::unique_ptr<Game> makeGame() const = 0;

		virtual void processInput(const sf::Event& evt, Game& game);
		virtual void update(const sf::Time& dt, Game& game);
		virtual void render(sf::RenderTarget& target, Game& game);

		std::unique_ptr<Game> mpGame;
	};
}

#endif
