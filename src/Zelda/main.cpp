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

	auto pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode{640, 480}, "Data-Oriented Design");

	ResourceManager<TMX> tmxManager{};
	ResourceManager<sf::Texture> textureManager{};
	ResourceManager<TextureAtlas> atlasManager{};
	ResourceManager<sf::Sprite> spriteManager{};

	auto tmxID = tmxManager.load("assets/maps/grassy.tmx");
	TMX& tmx{ tmxManager.get(tmxID) };
	std::vector<std::string> textureFilenames{};
	getTilesetFilenames(tmx, std::back_inserter(textureFilenames));
	std::vector<const sf::Texture*> textures;
	std::transform(textureFilenames.begin(), textureFilenames.end(), std::back_inserter(textures), [&textureManager](auto& filename) {
		return &textureManager.get(textureManager.load(filename));
	});
	std::vector<TileMapLayer> layers{};
	TileMapLayer::make(tmx, textures.begin(), textures.end(), std::back_inserter(layers));

	auto priestAtlasID = atlasManager.load("assets/spritesheets/priest/priest.xml");
	auto& atlas = atlasManager.get(priestAtlasID);
	auto priestTextureID = textureManager.load(atlas.getImagePath());
	auto spriteData = atlas.getSprite("clips/1-PriestWalkDown.png");
	auto pSprite = std::make_unique<sf::Sprite>(textureManager.get(priestTextureID),
		sf::IntRect{ spriteData.x, spriteData.y, spriteData.w, spriteData.h });
	pSprite->setOrigin(static_cast<float>(spriteData.w) * spriteData.pX, static_cast<float>(spriteData.h) * spriteData.pY);
	auto priestSpriteID = spriteManager.store(std::move(pSprite));

	EntityIDManager entityManager;
	GameData gameData;

	int playerID = 0;
	std::vector<TMX::Object> objects;
	getObjectsInGroup(tmx, "Entities", std::back_inserter(objects));
	for (auto& object : objects)
	{
		if (object.name == "Player")
		{
			playerID = entityManager.getNextID();
			gameData.sprites[playerID] = spriteManager.get(priestSpriteID);
			gameData.sortingLayers[playerID] = 1;
			gameData.positions[playerID] = { static_cast<float>(object.x), static_cast<float>(object.y) };
		}
	}

	ManagerRunner runner{ gameData, *pWindow };

	gameData.mapLayers[entityManager.getNextID()] = layers[0];

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	const sf::Time timePerFrame = sf::seconds(1.f / 60);

	while (pWindow->isOpen())
	{
		pWindow->clear();

		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;

			sf::Event evt;
			while (pWindow->pollEvent(evt))
			{
				if (evt.type == sf::Event::Closed)
				{
					pWindow->close();
				}
			}

			runner.fixedUpdate(timePerFrame);
		}

		runner.renderUpdate();
		pWindow->display();
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main(0, nullptr);
}
