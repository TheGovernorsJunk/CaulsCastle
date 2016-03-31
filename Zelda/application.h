#ifndef TE_APPLICATION_H
#define TE_APPLICATION_H

namespace sf
{
	class Time;
	class Event;
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
		virtual void processInput(const sf::Event& evt) = 0;
		virtual void update(const sf::Time& dt) = 0;
		virtual void render(sf::RenderTarget& target) = 0;
	};
}

#endif
