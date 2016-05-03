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
		StateMachine(EntityType& owner, std::unique_ptr<State<EntityType>>&& currentState, std::unique_ptr<State<EntityType>>&& globalState = nullptr)
			: mpOwner(owner)
			, mpCurrentState(std::move(currentState))
			, mpPreviousState(nullptr)
			, mpGlobalState(std::move(globalState))
		{
			if (mpCurrentState) mpCurrentState->enter(mpOwner);
			if (mpGlobalState) mpGlobalState->enter(mpOwner);
		}

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

		template <typename State, typename... Args>
		void changeState(Args&&... args)
		{
			mpPreviousState = std::move(mpCurrentState);
			mpPreviousState->exit(mpOwner);
			mpCurrentState = std::make_unique<State>(std::forward<Args>(args)...);
			mpCurrentState->enter(mpOwner);
		}

		void revertToPreviousState()
		{
			mpCurrentState->exit(mpOwner);
			auto upRevertState = std::move(mpPreviouState);
			mpPreviousState = std::move(mpCurrentState);
			mpCurrentState = std::move(upRevertState);
			mpCurrentState->enter(mpOwner);
		}
	private:
		EntityType& mpOwner;
		std::unique_ptr<State<EntityType>> mpCurrentState;
		std::unique_ptr<State<EntityType>> mpPreviousState;
		std::unique_ptr<State<EntityType>> mpGlobalState;
	};
}

#endif
