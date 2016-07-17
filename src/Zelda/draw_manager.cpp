#include "draw_manager.h"

namespace te
{
	DrawManager::DrawManager(const decltype(GameData::pixelToWorldScale)& pixelToWorldScale, const decltype(GameData::mainView)& mainView, decltype(GameData::pendingDraws)& pendingDraws, sf::RenderTarget& target)
		: m_rPixelToWorldScale{ pixelToWorldScale }
		, m_rMainView{ mainView }
		, m_PendingDraws{ pendingDraws }
		, m_Target{ target }
	{}

	void DrawManager::update()
	{
		sf::Vector2f scale{ 1.f / m_rPixelToWorldScale.x, 1.f / m_rPixelToWorldScale.y };

		std::sort(m_PendingDraws.begin(), m_PendingDraws.end(), [](auto a, auto b) {
			return a.drawOrder < b.drawOrder;
		});

		auto previousView = m_Target.getView();
		m_Target.setView(m_rMainView);
		for (auto& pendingDraw : m_PendingDraws)
		{
			pendingDraw.renderStates.transform.scale(scale);
			m_Target.draw(*pendingDraw.pDrawable, pendingDraw.renderStates);
		}
		m_Target.setView(previousView);

		m_PendingDraws.clear();
	}
}
