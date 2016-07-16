#include "draw_manager.h"

namespace te
{
	DrawManager::DrawManager(decltype(GameData::pendingDraws)& pendingDraws, sf::RenderTarget& target)
		: m_PendingDraws{ pendingDraws }
		, m_Target{ target }
	{}

	void DrawManager::update()
	{
		std::sort(m_PendingDraws.begin(), m_PendingDraws.end(), [](auto a, auto b) {
			return a.drawOrder < b.drawOrder;
		});
		for (auto& pendingDraw : m_PendingDraws)
		{
			m_Target.draw(*pendingDraw.pDrawable, pendingDraw.renderStates);
		}

		m_PendingDraws.clear();
	}
}
