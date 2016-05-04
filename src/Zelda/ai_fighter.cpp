#include "ai_fighter.h"
#include "goal_composite.h"
#include "goal_evaluator.h"
#include "message_dispatcher.h"

#include <iostream>

namespace te
{
	class FighterBrain : public GoalComposite<AIFighter>
	{
	public:
		FighterBrain(AIFighter& owner)
			: mOwner{owner}
			, mEvaluators{}
		{}

		void activate()
		{
			setStatus(Status::ACTIVE);
		}

		Status process(const sf::Time& dt)
		{
			return processSubgoals(dt);
		}

		void terminate() {}

		void arbitrate() {}

		bool handleMessage(const Telegram& telegram)
		{
			if (telegram.msg == Fighter::IncomingAttack)
			{
				std::cout << "AI received attack!" << std::endl;
				return true;
			}
			return false;
		}

	private:
		AIFighter& mOwner;
		std::vector<std::unique_ptr<GoalEvaluator<AIFighter>>> mEvaluators;
	};

	std::unique_ptr<AIFighter> AIFighter::make(BattleGame& world, sf::Vector2f pos)
	{
		return std::unique_ptr<AIFighter>{new AIFighter{world, pos}};
	}

	AIFighter::AIFighter(BattleGame& world, sf::Vector2f pos)
		: Fighter{world, pos}
		, mpBrain{std::make_unique<FighterBrain>(*this)}
	{}

	bool AIFighter::handleMessage(const Telegram& telegram)
	{
		return mpBrain->handleMessage(telegram) || Fighter::handleMessage(telegram);
	}

	void AIFighter::onUpdate(const sf::Time& dt)
	{
		mpBrain->process(dt);
		mpBrain->arbitrate();
		Fighter::onUpdate(dt);
	}
}
