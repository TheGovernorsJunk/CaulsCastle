#include "physics_world_manager.h"

namespace te
{
	PhysicsWorldManager::PhysicsWorldManager(b2World& world, const decltype(GameData::rigidBodies)& rigidBodies, decltype(GameData::positions)& positions)
		: m_rPhysicsWorld{ world }
		, m_rRigidBodies{ rigidBodies }
		, m_rPositions{ positions }
	{}

	void PhysicsWorldManager::update(const sf::Time& dt)
	{
		m_rPhysicsWorld.Step(dt.asSeconds(), 8, 3);
		for (auto& rigidBody : m_rRigidBodies)
		{
			auto entityID = rigidBody.first;
			auto position = rigidBody.second->GetPosition();
			m_rPositions[entityID] = sf::Vector2f{ position.x, position.y };
		}
	}
}
