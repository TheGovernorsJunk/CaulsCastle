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

//template <typename DrawableStore>
//class RenderManager
//{
//public:
//	RenderManager(DrawableStore& drawables,
//		ComponentStore<sf::Vector2f>& positions,
//		ComponentStore<int>& sortingLayers,
//		sf::RenderTarget& target)
//		: m_rDrawables{drawables}
//		, m_rPositions{positions}
//		, m_rSortingLayers{sortingLayers}
//		, m_rTarget{target}
//	{}
//
//	void update()
//	{
//		for (auto& entityDrawable : m_rDrawables)
//		{
//			int entityID = entityDrawable.first;
//			m_Indices.push_back({ entityID, m_rSortingLayers[entityID] });
//		}
//		std::sort(m_Indices.begin(), m_Indices.end(), [](auto a, auto b) {
//			return a.second < b.second;
//		});
//		for (auto& entitySortPair : m_Indices)
//		{
//			int entityID = entitySortPair.first;
//			auto& circle = m_rDrawables[entityID];
//			circle.setPosition(m_rPositions[entityID]);
//			m_rTarget.draw(circle);
//		}
//		m_Indices.clear();
//	}
//private:
//	DrawableStore& m_rDrawables;
//	ComponentStore<sf::Vector2f>& m_rPositions;
//	ComponentStore<int>& m_rSortingLayers;
//	sf::RenderTarget& m_rTarget;
//	std::vector<std::pair<int, int>> m_Indices;
//};


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

	ComponentStore<sf::Vector2f> positions{};
	ComponentStore<sf::CircleShape> circles{};
	ComponentStore<int> sortingLayers{};
	ComponentStore<TileMapLayer> mapLayers{};

	IncrementManager incrementManager{ positions };
	auto render = [&]() {
		static std::vector<std::tuple<int, int, sf::Drawable*>> indices{};

		for (auto& entitySortLayer : sortingLayers)
		{
			auto entityID = entitySortLayer.first;
			sf::Drawable* drawable = nullptr;
			if (circles.contains(entityID)) drawable = &circles[entityID];
			else if (mapLayers.contains(entityID)) drawable = &mapLayers[entityID];
			else throw std::runtime_error{"No drawable for entity " + entityID};
			indices.push_back({ entityID, entitySortLayer.second, drawable });
		}
		std::sort(indices.begin(), indices.end(), [](auto a, auto b) {
			return std::get<1>(a) < std::get<1>(b);
		});
		sf::RenderStates states;
		for (auto& entitySortPair : indices)
		{
			int entityID = std::get<0>(entitySortPair);
			auto& drawable = *std::get<2>(entitySortPair);
			states.transform = sf::Transform{}.translate(positions[entityID]);
			pWindow->draw(drawable, states);
		}
	};

	mapLayers[1] = layers[0];
	sortingLayers[1] = -1;

	positions[0] = { 30, 30 };
	sf::CircleShape circle0{ 20 };
	circle0.setFillColor(sf::Color::Magenta);
	circles[0] = std::move(circle0);
	sortingLayers[0] = 1;

	positions[3] = { 10, 10 };
	sf::CircleShape circle3{ 20 };
	circle3.setFillColor(sf::Color::Blue);
	circles[3] = std::move(circle3);
	sortingLayers[3] = 0;

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

		//layerRenderManager.update();
		//renderManager.update();
		render();
		pWindow->display();
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main(0, nullptr);
}
