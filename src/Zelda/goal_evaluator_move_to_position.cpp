#include "goal_evaluator_move_to_position.h"
#include "zelda_entity.h"
#include "goal_move_to_position.h"

namespace te
{
	float GoalEvaluator_MoveToPosition::calculateDesirability(const ZeldaEntity& entity)
	{
		return 1.f;
	}

	void GoalEvaluator_MoveToPosition::setGoal(ZeldaEntity& entity)
	{
		if (!entity.getSteering().isSeekEnabled())
		{
			entity.getBrain().addSubgoal<Goal_MoveToPosition>(entity, sf::Vector2f(16.f * 14, 16.f * 14));
		}
	}
}
