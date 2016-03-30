#include "goal_think.h"

namespace te
{
	void GoalThink::activate()
	{
		setStatus(Status::ACTIVE);
	}

	Goal<ZeldaEntity>::Status GoalThink::process()
	{
		return processSubgoals();
	}

	void GoalThink::terminate() {}
}
