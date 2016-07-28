#include "input_manager.h"

namespace te
{
	InputManager::InputManager(GameData& data)
		: m_rData{ data }
	{}

	void InputManager::update()
	{
		for (auto& inputPair : m_rData.playerInputs)
		{
			auto i = inputPair.first;
			auto& input = inputPair.second;
			input.switches = 0;

			auto keymap = m_rData.keymaps[i];
			if (sf::Joystick::isButtonPressed(i, static_cast<unsigned>(keymap.lightAttack)))
				input.switches = input.switches & InputSwitch::LIGHT_ATTACK;
			if (sf::Joystick::isButtonPressed(i, static_cast<unsigned>(keymap.parry)))
				input.switches = input.switches & InputSwitch::PARRY;

			input.lock = sf::Joystick::getAxisPosition(i, keymap.lock);
			input.heavyAttack = sf::Joystick::getAxisPosition(i, keymap.heavyAttack);
			input.x = sf::Joystick::getAxisPosition(i, keymap.X);
			input.y = sf::Joystick::getAxisPosition(i, keymap.Y);
		}
	}
}
