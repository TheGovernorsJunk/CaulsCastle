#include "input_manager.h"

namespace te
{
	InputManager::InputManager(GameData& data)
		: m_rData{ data }
	{}

	void InputManager::update()
	{
		size_t i = 0;
		for (auto& input : m_rData.playerInputs)
		{
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

			++i;
		}
	}
}
