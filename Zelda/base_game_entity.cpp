#include "base_game_entity.h"

namespace te
{
	BaseGameEntity::BaseGameEntity(const std::shared_ptr<Game>& pWorld)
		: mID(UNREGISTERED_ID)
		, mpWorld(pWorld)
	{}

	BaseGameEntity::~BaseGameEntity() {}

	float BaseGameEntity::getBoundingRadius() const
	{
		return 1.f;
	}

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
