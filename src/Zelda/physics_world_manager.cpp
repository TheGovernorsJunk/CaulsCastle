#include "physics_world_manager.h"

namespace te
{
	PhysicsWorldManager::PhysicsWorldManager(b2World& world, decltype(GameData::positions) positions)
		: m_rPhysicsWorld{ world }
		, m_rPositions{ positions }
	{}

	void PhysicsWorldManager::update(const sf::Time& dt)
	{
		m_rPhysicsWorld.Step(dt.asSeconds(), 8, 3);
	}
}
