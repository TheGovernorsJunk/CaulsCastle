#include "player_manager.h"
#include "vector_ops.h"

#include <type_traits>

namespace te
{
	PlayerManager::PlayerManager(EntityID entityID, const decltype(GameData::directionInput)& input, decltype(GameData::velocities)& velocities)
		: m_ID{ entityID }
		, m_rInput{ input }
		, m_rVelocities{ velocities }
	{}

	void PlayerManager::update()
	{
		std::remove_reference_t<decltype(m_rVelocities[0])> velocity;
		velocity.x = m_rInput.left && !m_rInput.right ? -1.f
			: !m_rInput.left && m_rInput.right ? 1.f
			: 0;
		velocity.y = m_rInput.up && !m_rInput.down ? -1.f
			: !m_rInput.up && m_rInput.down ? 1.f
			: 0;
		m_rVelocities[m_ID] = normalize(velocity) * 5.f;
	}
}
