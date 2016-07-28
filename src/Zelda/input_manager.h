#ifndef TE_INPUT_MANAGER_H
#define TE_INPUT_MANAGER_H

#include "game_data.h"

#include <SFML/Graphics.hpp>

#include <Windows.h>
#include <Xinput.h>

namespace te
{
	class InputManager
	{
	public:
		InputManager(GameData& data);

		void update();
	private:
		GameData& m_rData;
		XINPUT_STATE m_XIState;
	};
}

#endif
