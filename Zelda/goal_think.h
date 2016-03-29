#ifndef TE_GOAL_THINK_H
#define TE_GOAL_THINK_H

#include "goal_composite.h"

namespace te
{
	class ZeldaEntity;

	class GoalThink : public GoalComposite<ZeldaEntity>
	{
		void activate();
		Status process();
		void terminate();
		bool handleMessage(const Telegram&);
		void addSubgoal(const Goal&) {}
	};
}

#endif
