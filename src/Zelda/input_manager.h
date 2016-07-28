#ifndef TE_INPUT_MANAGER_H
#define TE_INPUT_MANAGER_H

#include "game_data.h"

#include <SFML/Graphics.hpp>

namespace te
{
	class InputManager
	{
	public:
		InputManager(GameData& data);

		void update();
	private:
		GameData& m_rData;
	};
}

#endif
