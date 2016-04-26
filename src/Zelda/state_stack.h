#ifndef TE_STATE_STACK_H
#define TE_STATE_STACK_H

#include "runnable.h"
#include "game_state.h"

#include <vector>
#include <memory>

namespace te
{
	class StateStack : public Runnable
	{
	public:
		template<typename T, typename... Args>
		void queuePush(Args&&... args)
		{
			mPendingActions.push_back(Action{
				ActionType::Push,
				T::make(std::forward<Args>(args)...)
			});
		}

		void queuePop();
		void queueClear();

		void processInput(const sf::Event&);
		void update(const sf::Time&);

		void processPendingActions()
		{
			for (auto& action : mPendingActions)
			{
				switch (action.type)
				{
				case ActionType::Push:
					mStack.push_back(std::move(action.pState));
					break;
				case ActionType::Pop:
					mStack.pop_back();
					break;
				case ActionType::Clear:
					mStack.clear();
					break;
				}
			}

			mPendingActions.clear();
		}

	private:
		enum class ActionType { Push, Pop, Clear };
		struct Action
		{
			ActionType type;
			std::unique_ptr<GameState> pState;
		};

		void draw(sf::RenderTarget&, sf::RenderStates) const;

		std::vector<std::unique_ptr<GameState>> mStack;

		std::vector<Action> mPendingActions;
	};
}

#endif
