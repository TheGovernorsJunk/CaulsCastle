#include "scripted_application.h"

#include <LuaBridge.h>

#include <iostream>
#include <Windows.h>

#include <SFML/Graphics.hpp>
#include <boost/container/flat_map.hpp>
#include <vector>

template <typename Component>
class ComponentStore
{
public:
	auto begin() { return m_Components.begin(); }
	auto end() { return m_Components.end(); }
	auto cbegin() { return m_Components.cbegin(); }
	auto cend() { return m_Components.cend(); }

	Component& operator[](int index)
	{
		return m_Components[index];
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

class RenderManager
{
public:
	RenderManager(ComponentStore<sf::CircleShape>& circles,
		ComponentStore<sf::Vector2f>& positions,
		ComponentStore<int>& sortingLayers,
		sf::RenderTarget& target)
		: m_rCircles{circles}
		, m_rPositions{positions}
		, m_rSortingLayers{sortingLayers}
		, m_rTarget{target}
	{}

	void update()
	{
		for (auto& entityCircle : m_rCircles)
		{
			int entityID = entityCircle.first;
			m_Indices.push_back({ entityID, m_rSortingLayers[entityID] });
			//auto& circle = entityCircle.second;
			//circle.setPosition(m_rPositions[entityCircle.first]);
			//m_rTarget.draw(circle);
		}
		std::sort(m_Indices.begin(), m_Indices.end(), [](auto a, auto b) {
			return a.second < b.second;
		});
		for (auto& entitySortPair : m_Indices)
		{
			int entityID = entitySortPair.first;
			auto& circle = m_rCircles[entityID];
			circle.setPosition(m_rPositions[entityID]);
			m_rTarget.draw(circle);
		}
		m_Indices.clear();
	}
private:
	ComponentStore<sf::CircleShape>& m_rCircles;
	ComponentStore<sf::Vector2f>& m_rPositions;
	ComponentStore<int>& m_rSortingLayers;
	sf::RenderTarget& m_rTarget;
	std::vector<std::pair<int, int>> m_Indices;
};


int main(int argc, char* argv[])
{
	auto pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode{640, 480}, "Data-Oriented Design");

	ComponentStore<sf::Vector2f> positions{};
	ComponentStore<sf::CircleShape> circles{};
	ComponentStore<int> sortingLayers{};

	IncrementManager incrementManager{ positions };
	RenderManager renderManager{ circles, positions, sortingLayers, *pWindow };
	//flat_map<int, sf::Vector2f> velocities{};

	positions[0] = { 30, 30 };
	positions[3] = { 10, 10 };

	sf::CircleShape circle0{ 10 };
	circle0.setFillColor(sf::Color::Magenta);
	circles[0] = std::move(circle0);

	sf::CircleShape circle3{ 20 };
	circle3.setFillColor(sf::Color::Blue);
	circles[3] = std::move(circle3);

	sortingLayers[0] = 2;
	sortingLayers[3] = 1;

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
