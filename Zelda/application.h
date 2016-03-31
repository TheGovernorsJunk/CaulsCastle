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
	class Application
	{
	public:
		virtual ~Application();

		void run(int fps = 60);
	private:
		virtual std::unique_ptr<sf::RenderWindow> makeWindow() const = 0;
		virtual void processInput(const sf::Event& evt) = 0;
		virtual void update(const sf::Time& dt) = 0;
		virtual void render(sf::RenderTarget& target) = 0;
	};
}

#endif
