#include "velocity_manager.h"

namespace te
{
	VelocityManager::VelocityManager(
		decltype(GameData::positions)& positions,
		const decltype(GameData::velocities)& velocities)
		: m_rPositions{ positions }
		, m_rVelocities{ velocities }
	{}

	void VelocityManager::update(const sf::Time& dt)
	{
		for (auto& entityVelocity : m_rVelocities)
		{
			auto id = entityVelocity.first;
			auto velocity = entityVelocity.second;
			m_rPositions[id] += velocity * dt.asSeconds();
		}
	}
}
