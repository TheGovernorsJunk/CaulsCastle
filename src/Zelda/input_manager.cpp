#include "input_manager.h"

namespace te
{
	InputManager::InputManager(GameData& data)
		: m_rData{ data }
		, m_XIState{}
	{
		ZeroMemory(&m_XIState, sizeof(XINPUT_STATE));
	}

	static inline float getAxisPosition(XINPUT_STATE& state, unsigned i, sf::Joystick::Axis axis)
	{
		if (axis == (unsigned)XBoxInput::LT)
		{
			XInputGetState(i, &state);
			return state.Gamepad.bLeftTrigger / 2.55f;
		}
		if (axis == (unsigned)XBoxInput::RT)
		{
			XInputGetState(i, &state);
			return state.Gamepad.bRightTrigger / 2.55f;
		}
		return sf::Joystick::getAxisPosition(i, axis);
	}

	void InputManager::update()
	{
		for (auto& mapPair : m_rData.keymaps)
		{
			auto i = mapPair.first;
			auto keymap = mapPair.second;
			auto& input = m_rData.playerInputs[i];

			input.switches = 0;

			if (sf::Joystick::isButtonPressed(i, static_cast<unsigned>(keymap.lightAttack)))
				input.switches = input.switches & InputSwitch::LIGHT_ATTACK;
			if (sf::Joystick::isButtonPressed(i, static_cast<unsigned>(keymap.parry)))
				input.switches = input.switches & InputSwitch::PARRY;

			input.lock = getAxisPosition(m_XIState, i, keymap.lock);
			input.heavyAttack = getAxisPosition(m_XIState, i, keymap.heavyAttack);
			input.x = getAxisPosition(m_XIState, i, keymap.X);
			input.y = getAxisPosition(m_XIState, i, keymap.Y);
		}
	}
}
