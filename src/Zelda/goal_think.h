#ifndef TE_GOAL_THINK_H
#define TE_GOAL_THINK_H

#include "goal_composite.h"
#include "goal_evaluator.h"

namespace te
{
	class ZeldaEntity;

	class GoalThink : public GoalComposite<ZeldaEntity>
	{
	public:
		GoalThink(ZeldaEntity& owner);

		void activate();
		Status process(const sf::Time& dt);
		void terminate();
		//bool handleMessage(const Telegram&);

		void arbitrate();

	private:
		ZeldaEntity& mOwner;
		std::list<std::unique_ptr<GoalEvaluator<ZeldaEntity>>> mEvaluators;
	};
}

#endif
