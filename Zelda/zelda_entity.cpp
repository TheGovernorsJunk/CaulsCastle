#include "zelda_entity.h"

namespace te
{
	ZeldaEntity::ZeldaEntity(const std::shared_ptr<Game>& pGame)
		: MovingEntity(pGame)
		, mPathPlanner(*this)
		, mBrain(*this)
	{}
}
