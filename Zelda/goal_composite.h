#ifndef TE_GOAL_COMPOSITE_H
#define TE_GOAL_COMPOSITE_H

#include "goal.h"

#include <list>
#include <memory>

namespace te
{
	template<class T>
	class GoalComposite : public Goal<T>
	{
	public:
		template <class U>
		void addSubgoal()
		{
			std::unique_ptr<Goal<T>> pGoal(new U());
			mSubgoals.push_front(std::move(pGoal));
		}

	protected:
		Status processSubgoals()
		{
			while (!mSubgoals.empty() && (mSubgoals.front()->isCompleted() || mSubgoals.front()->hasFailed()))
			{
				mSubgoals.front()->terminate();
				mSubgoals.pop_front();
			}

			if (!mSubgoals.empty())
			{
				Status statusOfSubgoals = mSubgoals.front()->process();

				if (statusOfSubgoals == Status::COMPLETED && mSubgoals.size() > 1)
				{
					return Status::ACTIVE;
				}

				return statusOfSubgoals;
			}
			else
			{
				return Status::COMPLETED;
			}
		}

		void removeAllSubgoals()
		{
			for (auto& pGoal : mSubgoals)
			{
				pGoal->terminate();
			}
			mSubgoals.clear();
		}

	private:
		std::list<std::unique_ptr<Goal<T>>> mSubgoals;
	};
}

#endif
