#include "player_manager.h"

namespace te
{
	PlayerManager::PlayerManager(EntityID entityID, const decltype(GameData::directionInput)& input, decltype(GameData::velocities)& velocities)
		: m_ID{ entityID }
		, m_rInput{ input }
		, m_rVelocities{ velocities }
	{}

	void PlayerManager::update()
	{
		sf::Vector2f velocity{};
		velocity.x = m_rInput.left && !m_rInput.right ? -5.f
			: !m_rInput.left && m_rInput.right ? 5.f
			: 0;
		velocity.y = m_rInput.up && !m_rInput.down ? -5.f
			: !m_rInput.up && m_rInput.down ? 5.f
			: 0;
		m_rVelocities[m_ID] = velocity;
	}
}
