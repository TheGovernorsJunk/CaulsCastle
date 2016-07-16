#include "velocity_manager.h"

namespace te
{
	VelocityManager::VelocityManager(
		const decltype(GameData::velocities)& velocities,
		decltype(GameData::rigidBodies)& rigidBodies,
		decltype(GameData::positions)& positions)
		: m_rVelocities{ velocities }
		, m_rPositions{ positions }
		, m_rRigidBodies{ rigidBodies }
	{}

	void VelocityManager::update(const sf::Time& dt)
	{
		for (auto& entityVelocity : m_rVelocities)
		{
			auto id = entityVelocity.first;
			auto velocity = entityVelocity.second;
			if (m_rRigidBodies.contains(id))
				m_rRigidBodies[id]->SetLinearVelocity({ velocity.x, velocity.y });
			else
				m_rPositions[id] += velocity * dt.asSeconds();
		}
	}
}
