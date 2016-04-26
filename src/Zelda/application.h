#ifndef TE_APPLICATION_H
#define TE_APPLICATION_H

#include "typedefs.h"

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
	class TextureManager;
	class TextureAtlas;
	class Runnable;

	class Application
	{
	public:
		Application();
		virtual ~Application();

		TextureManager& getTextureManager() const;

		void run(int fps = 60);
	private:
		virtual std::unique_ptr<sf::RenderWindow> makeWindow() const = 0;
		virtual std::unique_ptr<Runnable> makeRunnable() = 0;

		virtual void processInput(const sf::Event& evt, Runnable& game);
		virtual void update(const sf::Time& dt, Runnable& game);
		virtual void render(sf::RenderTarget& target, Runnable& game);

		std::unique_ptr<TextureManager> mpTextureManager;
	};
}

#endif
