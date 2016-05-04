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
		template <class U, class... Args>
		void addSubgoal(Args&&... args)
		{
			mSubgoals.push_front(std::make_unique<U>(std::forward<Args>(args)...));
		}

		bool handleMessage(const Telegram& telegram)
		{
			if (!mSubgoals.empty() && !mSubgoals.front()->isCompleted() && !mSubgoals.front()->hasFailed())
				return mSubgoals.front()->handleMessage(telegram);
			else
				return false;
		}

	protected:
		Status processSubgoals(const sf::Time& dt)
		{
			while (!mSubgoals.empty() && (mSubgoals.front()->isCompleted() || mSubgoals.front()->hasFailed()))
			{
				mSubgoals.front()->terminate();
				mSubgoals.pop_front();
			}

			if (!mSubgoals.empty())
			{
				Status statusOfSubgoals = mSubgoals.front()->process(dt);

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
