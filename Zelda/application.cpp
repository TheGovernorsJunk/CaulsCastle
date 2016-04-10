#include "application.h"
#include "game.h"
#include "texture_manager.h"
#include "texture_atlas.h"

#include <SFML/Graphics.hpp>

namespace te
{
	Application::Application()
		: mpTextureManager(TextureManager::make())
		, mTextureAtlasMap()
	{
		auto pAtlas = TextureAtlas::make("textures", "inigo_spritesheet.xml", mpTextureManager.get());
		TextureID id = pAtlas->getTextureID();
		mTextureAtlasMap.insert({ id, std::move(pAtlas) });
	}
	Application::~Application() {}

	void Application::run(int fps)
	{
		auto window = makeWindow();
		window->setKeyRepeatEnabled(false);

		auto pGame = makeGame();

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
						processInput(evt, *pGame);
					}
				}

				update(timePerFrame, *pGame);
			}

			window->clear();
			render(*window, *pGame);
			window->display();
		}
	}

	TextureManager& Application::getTextureManager() const
	{
		return *mpTextureManager;
	}

	const TextureAtlas& Application::getAtlas(TextureID id) const
	{
		auto it = mTextureAtlasMap.find(id);
		if (it != mTextureAtlasMap.end())
		{
			return *it->second;
		}
		throw std::runtime_error("No atlas for given ID.");
	}

	void Application::processInput(const sf::Event& evt, Game& game)
	{
		game.processInput(evt);
	}
	void Application::update(const sf::Time& dt, Game& game)
	{
		game.update(dt);
	}
	void Application::render(sf::RenderTarget& target, Game& game)
	{
		target.draw(game);
	}
}
