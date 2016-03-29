#ifndef TE_ZELDA_ENTITY_H
#define TE_ZELDA_ENTITY_H

#include "moving_entity.h"
#include "path_planner.h"

namespace te
{
	class Game;

	class ZeldaEntity : public MovingEntity
	{
	public:
		ZeldaEntity(const std::shared_ptr<Game>& pGame);

	private:
		PathPlanner mPathPlanner;
	};
}

#endif
