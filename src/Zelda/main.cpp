#define NOMINMAX
#include "scripted_application.h"

#include <LuaBridge.h>

#include <iostream>
#include <Windows.h>

#include "component_store.h"
#include "game_data.h"
#include "entity_id_manager.h"
#include "resource_manager.h"
#include "tmx.h"
#include "utilities.h"
#include "tile_map_layer.h"
#include "texture_atlas.h"
#include "manager_runner.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iterator>
#include <algorithm>
#include <unordered_map>
#include <memory>

int main(int argc, char* argv[])
{
	using namespace te;

	GameData gameData;

	ManagerRunner runner{ gameData };


	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const sf::Time timePerFrame = sf::seconds(1.f / gameData.config.fps);

	while (gameData.pWindow->isOpen())
	{
		gameData.pWindow->clear();

		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			sf::Event evt;
			while (gameData.pWindow->pollEvent(evt))
			{
				if (evt.type == sf::Event::Closed)
				{
					gameData.pWindow->close();
				}
			}

			runner.fixedUpdate(timePerFrame);
		}

		runner.renderUpdate();
		gameData.pWindow->display();
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main(0, nullptr);
}
