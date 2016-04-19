#ifndef TE_GOAL_EVALUATOR_H
#define TE_GOAL_EVALUATOR_H

namespace te
{
	template <class T>
	class GoalEvaluator
	{
	public:
		virtual ~GoalEvaluator() {}

		virtual float calculateDesirability(const T& entity) = 0;
		virtual void setGoal(T& entity) = 0;
	};
}

#endif
