#ifndef TE_GOAL_COMPOSITE_H
#define TE_GOAL_COMPOSITE_H

#include "goal.h"

#include <memory>

namespace te
{
	template<class T>
	class GoalComposite : public Goal<T>
	{
	public:
	private:
		std::list<std::unique_ptr<Goal<T>>> mSubgoals;
	};
}

#endif
