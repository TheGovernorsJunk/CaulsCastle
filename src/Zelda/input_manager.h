#ifndef TE_INPUT_MANAGER_H
#define TE_INPUT_MANAGER_H

#include "game_data.h"

#include <SFML/Graphics.hpp>

namespace te
{
	class InputManager
	{
	public:
		InputManager(decltype(GameData::directionInput)& input);

		void update();
	private:
		decltype(GameData::directionInput)& m_rInput;
	};
}

#endif
