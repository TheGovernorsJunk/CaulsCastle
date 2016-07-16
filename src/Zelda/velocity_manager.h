#ifndef TE_VELOCITY_MANAGER_H
#define TE_VELOCITY_MANAGER_H

#include "game_data.h"

namespace te
{
	class VelocityManager
	{
	public:
		VelocityManager(
			const decltype(GameData::velocities)& velocities,
			decltype(GameData::rigidBodies)& rigidBodies,
			decltype(GameData::positions)& positions);

		void update(const sf::Time& dt);
	private:
		const decltype(GameData::velocities)& m_rVelocities;
		decltype(GameData::positions)& m_rPositions;
		decltype(GameData::rigidBodies)& m_rRigidBodies;
	};
}

#endif
