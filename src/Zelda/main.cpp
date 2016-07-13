#include "scripted_application.h"

#include <LuaBridge.h>

#include <iostream>
#include <Windows.h>

#include <SFML/Graphics.hpp>
#include <vector>

template <typename Component>
class ComponentStore
{
public:
	auto begin() { return m_Components.begin(); }
	auto end() { return m_Components.end(); }
	auto cbegin() { return m_Components.cbegin(); }
	auto cend() { return m_Components.cend(); }

	Component& operator[](unsigned index)
	{
		if (m_Components.size() <= index) m_Components.resize(index + 1);
		return m_Components[index];
	}
private:
	std::vector<Component> m_Components;
};

class IncrementManager
{
public:
	IncrementManager(ComponentStore<sf::Vector2f>& positions)
		: m_rPositions{positions}
	{}

	void update()
	{
		for (auto& position : m_rPositions)
		{
			position += { 1, 1 };
		}
	}
private:
	ComponentStore<sf::Vector2f>& m_rPositions;
};

int main(int argc, char* argv[])
{
	ComponentStore<sf::Vector2f> positions{};
	IncrementManager incrementManager{ positions };

	positions[0] = { -5000, -5000 };
	positions[3] = { 10, 10 };

	auto pWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode{640, 480}, "Data-Oriented Design");
	while (pWindow->isOpen())
	{
		sf::Event evt;
		while (pWindow->pollEvent(evt))
		{
			if (evt.type == sf::Event::Closed)
			{
				pWindow->close();
			}
		}

		incrementManager.update();
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return main(0, nullptr);
}
