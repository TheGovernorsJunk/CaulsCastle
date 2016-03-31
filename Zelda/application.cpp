#include "application.h"
#include <SFML/Graphics.hpp>

namespace te
{
	Application::~Application() {}

	void Application::run(int fps)
	{
		auto window = makeWindow();
		window->setKeyRepeatEnabled(false);

		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;
		const sf::Time timePerFrame = sf::seconds(1.f / fps);

		while (window->isOpen())
		{
			sf::Time dt = clock.restart();
			timeSinceLastUpdate += dt;
			while (timeSinceLastUpdate > timePerFrame)
			{
				timeSinceLastUpdate -= timePerFrame;

				sf::Event evt;
				while (window->pollEvent(evt))
				{
					if (evt.type == sf::Event::Closed)
					{
						window->close();
					}
					else
					{
						processInput(evt);
					}
				}

				update(timePerFrame);
			}

			window->clear();
			window->display();
		}
	}
}
