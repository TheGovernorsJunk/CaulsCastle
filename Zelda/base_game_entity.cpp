#include "base_game_entity.h"

namespace te
{
	BaseGameEntity::BaseGameEntity()
		: mID(0) {}

	int BaseGameEntity::getID() const
	{
		return mID;
	}
}
