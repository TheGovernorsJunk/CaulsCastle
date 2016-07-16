#ifndef TE_DRAW_MANAGER_H
#define TE_DRAW_MANAGER_H

#include "game_data.h"

#include <SFML/Graphics.hpp>

namespace te
{
	class DrawManager
	{
	public:
		DrawManager(decltype(GameData::pendingDraws)& pendingDraws, sf::RenderTarget& target);

		void update();
	private:
		decltype(GameData::pendingDraws)& m_PendingDraws;
		sf::RenderTarget& m_Target;
	};
}

#endif
