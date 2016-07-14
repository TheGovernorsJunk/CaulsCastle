#define NOMINMAX
#include "scripted_application.h"

#include <LuaBridge.h>

#include <iostream>
#include <Windows.h>

#include "resource_manager.h"
#include "tmx.h"
#include "utilities.h"
#include "tile_map_layer.h"
#include <SFML/Graphics.hpp>
#include <boost/container/flat_map.hpp>
#include <vector>
#include <iterator>
#include <algorithm>

template <typename Component>
class ComponentStore
{
public:
	auto begin() { return m_Components.begin(); }
	auto end() { return m_Components.end(); }
	auto cbegin() { return m_Components.cbegin(); }
	auto cend() { return m_Components.cend(); }

	inline Component& operator[](int index)
	{
		return m_Components[index];
	}

	inline bool contains(int index) const
	{
		return m_Components.find(index) != m_Components.end();
	}
private:
	boost::container::flat_map<int, Component> m_Components;
};

class IncrementManager
{
public:
	IncrementManager(ComponentStore<sf::Vector2f>& positions)
		: m_rPositions{positions}
	{}

	void update(const sf::Time& dt)
	{
		for (auto& entityPosition : m_rPositions)
		{
			entityPosition.second += sf::Vector2f{ 10.f, 10.f } * dt.asSeconds();
		}
	}
private:
	ComponentStore<sf::Vector2f>& m_rPositions;
};

struct GameData
{
	ComponentStore<sf::Vector2f> positions;
	ComponentStore<sf::CircleShape> circles;
	ComponentStore<int> sortingLayers;
	ComponentStore<te::TileMapLayer> mapLayers;
};

class RenderManager
{
public:
	RenderManager(GameData& data, sf::RenderTarget& target)
		: m_Data(data)
		, m_Target(target)
	{}

	void update() {
		for (auto& entitySortLayer : m_Data.sortingLayers)
		{
			auto entityID = entitySortLayer.first;
			sf::Drawable* drawable = nullptr;
			if (m_Data.circles.contains(entityID)) drawable = &m_Data.circles[entityID];
			else if (m_Data.mapLayers.contains(entityID)) drawable = &m_Data.mapLayers[entityID];
			else throw std::runtime_error{ "No drawable for entity " + entityID };
			m_PendingDraws.push_back({ entityID, entitySortLayer.second, drawable });
		}
		std::sort(m_PendingDraws.begin(), m_PendingDraws.end(), [](auto a, auto b) {
			return a.drawOrder < b.drawOrder;
		});
		sf::RenderStates states;
		for (auto& pendingDraw : m_PendingDraws)
		{
			int entityID = pendingDraw.entityID;
			auto& drawable = *pendingDraw.pDrawable;
			states.transform = sf::Transform{}.translate(m_Data.positions[entityID]);
			m_Target.draw(drawable, states);
		}

		m_PendingDraws.clear();
	};

private:
	struct PendingDraw
	{
		int entityID;
		int drawOrder;
		sf::Drawable* pDrawable;
	};
	GameData& m_Data;
	sf::RenderTarget& m_Target;
	std::vector<PendingDraw> m_PendingDraws;
};

int main(int argc, char* argv[])
{
	using namespace te;

	auto pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode{640, 480}, "Data-Oriented Design");

	ResourceManager<TMX> tmxManager{};
	auto id = tmxManager.load("assets/maps/grassy.tmx");
	std::vector<std::string> textureFilenames{};
	getTilesetFilenames(tmxManager.get(id), std::back_inserter(textureFilenames));
	ResourceManager<sf::Texture> textureManager{};
	std::vector<const sf::Texture*> textures;
	std::transform(textureFilenames.begin(), textureFilenames.end(), std::back_inserter(textures), [&textureManager](auto& filename) {
		return &textureManager.get(textureManager.load(filename));
	});
	std::vector<TileMapLayer> layers{};
	TileMapLayer::make(tmxManager.get(id), textures.begin(), textures.end(), std::back_inserter(layers));

	GameData gameData;

	IncrementManager incrementManager{ gameData.positions };
	RenderManager renderManager{ gameData, *pWindow };

	gameData.mapLayers[1] = layers[0];
	gameData.sortingLayers[1] = -1;

	gameData.positions[0] = { 30, 30 };
	sf::CircleShape circle0{ 20 };
	circle0.setFillColor(sf::Color::Magenta);
	gameData.circles[0] = std::move(circle0);
	gameData.sortingLayers[0] = 1;

	gameData.positions[3] = { 10, 10 };
	sf::CircleShape circle3{ 20 };
	circle3.setFillColor(sf::Color::Blue);
	gameData.circles[3] = std::move(circle3);
	gameData.sortingLayers[3] = 0;

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

			incrementManager.update(timePerFrame);
		}

		renderManager.update();
		pWindow->display();
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main(0, nullptr);
}
