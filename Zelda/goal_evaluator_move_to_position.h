#ifndef TE_GOAL_EVALUATOR_MOVE_TO_POSITION_H
#define TE_GOAL_EVALUATOR_MOVE_TO_POSITION_H

#include "goal_evaluator.h"

namespace te
{
	class ZeldaEntity;

	class GoalEvaluator_MoveToPosition : public GoalEvaluator<ZeldaEntity>
	{
	public:
		float calculateDesirability(const ZeldaEntity& entity);
		void setGoal(ZeldaEntity& entity);
	};
}

#endif
