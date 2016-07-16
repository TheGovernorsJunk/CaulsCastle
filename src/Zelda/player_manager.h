#ifndef TE_PLAYER_MANAGER_H
#define TE_PLAYER_MANAGER_H

#include "game_data.h"
#include "typedefs.h"

namespace te
{
	class PlayerManager
	{
	public:
		PlayerManager(EntityID entityID, const decltype(GameData::directionInput)& input, decltype(GameData::velocities)& velocities);

		void update();
	private:
		EntityID m_ID;
		const decltype(GameData::directionInput)& m_rInput;
		decltype(GameData::velocities)& m_rVelocities;
	};
}

#endif
