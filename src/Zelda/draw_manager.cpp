#include "draw_manager.h"

namespace te
{
	DrawManager::DrawManager(const decltype(GameData::pixelToWorldScale)& pixelToWorldScale, decltype(GameData::pendingDraws)& pendingDraws, sf::RenderTarget& target)
		: m_rPixelToWorldScale{ pixelToWorldScale }
		, m_PendingDraws{ pendingDraws }
		, m_Target{ target }
	{}

	void DrawManager::update()
	{
		sf::Vector2f scale{ 1.f / m_rPixelToWorldScale.x, 1.f / m_rPixelToWorldScale.y };

		std::sort(m_PendingDraws.begin(), m_PendingDraws.end(), [](auto a, auto b) {
			return a.drawOrder < b.drawOrder;
		});
		for (auto& pendingDraw : m_PendingDraws)
		{
			pendingDraw.renderStates.transform.scale(scale);
			m_Target.draw(*pendingDraw.pDrawable, pendingDraw.renderStates);
		}

		m_PendingDraws.clear();
	}
}
