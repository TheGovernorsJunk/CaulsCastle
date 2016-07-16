#ifndef TE_VELOCITY_MANAGER_H
#define TE_VELOCITY_MANAGER_H

#include "game_data.h"

namespace te
{
	class VelocityManager
	{
	public:
		VelocityManager(
			decltype(GameData::positions)& positions,
			const decltype(GameData::velocities)& velocities);

		void update(const sf::Time& dt);
	private:
		decltype(GameData::positions)& m_rPositions;
		const decltype(GameData::velocities)& m_rVelocities;
	};
}

#endif
