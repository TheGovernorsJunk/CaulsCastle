#include "base_game_entity.h"

namespace te
{
	BaseGameEntity::BaseGameEntity()
		: mID(UNREGISTERED_ID) {}

	BaseGameEntity::~BaseGameEntity() {}

	int BaseGameEntity::getID() const
	{
		return mID;
	}
}
