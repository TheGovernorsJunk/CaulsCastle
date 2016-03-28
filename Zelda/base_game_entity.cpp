#include "base_game_entity.h"

namespace te
{
	BaseGameEntity::BaseGameEntity(const std::shared_ptr<Game>& pWorld)
		: mID(UNREGISTERED_ID)
		, mpWorld(pWorld)
	{}

	BaseGameEntity::~BaseGameEntity() {}

	int BaseGameEntity::getID() const
	{
		return mID;
	}

	const Game& BaseGameEntity::getWorld() const
	{
		return *mpWorld;
	}

	Game& BaseGameEntity::getWorld()
	{
		return *mpWorld;
	}
}
