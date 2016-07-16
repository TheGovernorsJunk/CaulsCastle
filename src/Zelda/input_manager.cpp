#include "input_manager.h"

namespace te
{
	InputManager::InputManager(decltype(GameData::directionInput)& input)
		: m_rInput{ input }
	{}

	void InputManager::update()
	{
		m_rInput.left = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		m_rInput.right = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		m_rInput.up = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		m_rInput.down = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	}
}
