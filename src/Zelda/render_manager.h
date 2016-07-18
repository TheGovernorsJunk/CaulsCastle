#ifndef TE_RENDER_MANAGER_H
#define TE_RENDER_MANAGER_H

#include "game_data.h"

namespace te
{
	template <typename DrawableStore>
	class RenderManager
	{
	public:
		RenderManager(const DrawableStore& drawableStore,
			decltype(GameData::positions)& positionStore,
			decltype(GameData::pendingDraws)& pendingDraws)
			: m_DrawableStore(drawableStore)
			, m_PositionStore(positionStore)
			, m_PendingDraws(pendingDraws)
		{}
		RenderManager(RenderManager&&) = default;
		RenderManager& operator=(RenderManager&&) = default;

		void update()
		{
			std::transform(std::cbegin(m_DrawableStore), std::cend(m_DrawableStore), std::back_inserter(m_PendingDraws), [this](const auto& entityDrawable) {
				auto entityID = entityDrawable.first;
				const auto& drawable = entityDrawable.second;
				sf::RenderStates renderStates;
				renderStates.transform.translate(m_PositionStore[entityID]);
				return PendingDraw{ renderStates, drawable.sortingLayer, &drawable.drawable };
			});
		}

	private:
		const DrawableStore& m_DrawableStore;
		decltype(GameData::positions)& m_PositionStore;
		decltype(GameData::pendingDraws)& m_PendingDraws;
	};

	template <typename Drawables>
	auto makeRenderManager(const Drawables& drawables,
		decltype(GameData::positions)& positions,
		decltype(GameData::sortingLayers)& sortings,
		decltype(GameData::pendingDraws)& pendingDraws)
	{
		return RenderManager<Drawables>{ drawables, positions, sortings, pendingDraws };
	}
}

#endif
