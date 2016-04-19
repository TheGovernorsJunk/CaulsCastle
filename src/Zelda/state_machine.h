#ifndef TE_STATE_MACHINE_H
#define TE_STATE_MACHINE_H

#include "state.h"

#include <memory>

namespace te
{
	struct Telegram;

	template <class EntityType>
	class StateMachine
	{
	public:
		StateMachine(EntityType& owner, const std::shared_ptr<State<EntityType>>& currentState, const std::shared_ptr<State<EntityType>>& globalState = nullptr)
			: mpOwner(owner)
			, mpCurrentState(currentState)
			, mpPreviousState(nullptr)
			, mpGlobalState(globalState)
		{}

		//void setCurrentState(State<EntityType>* state) { mpCurrentState = state; }
		//void setGlobalState(State<EntityType>* state) { mpGlobalState = state; }
		//void setPreviousState(State<EntityType>* state) { mpPreviousState = state; }

		void update(const sf::Time& dt) const
		{
			if (mpGlobalState)
			{
				mpGlobalState->execute(mpOwner, dt);
			}
			if (mpCurrentState)
			{
				mpCurrentState->execute(mpOwner, dt);
			}
		}

		bool handleMessage(const Telegram& telegram) const
		{
			if (mpCurrentState && mpCurrentState->onMessage(mpOwner, telegram))
			{
				return true;
			}

			return (mpGlobalState && mpGlobalState->onMessage(mpOwner, telegram));
		}

		void changeState(State<EntityType>& pNewState)
		{
			mpPreviousState = mpCurrentState;
			mpCurrentState->exit(mpOwner);
			mpCurrentState = &pNewState;
			mpCurrentState->enter(mpOwner);
		}

		void revertToPreviousState()
		{
			changeState(mpPreviousState);
		}
	private:
		EntityType& mpOwner;
		std::shared_ptr<State<EntityType>> mpCurrentState;
		std::shared_ptr<State<EntityType>> mpPreviousState;
		std::shared_ptr<State<EntityType>> mpGlobalState;
	};
}

#endif
