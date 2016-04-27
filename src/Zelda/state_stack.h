#ifndef TE_STATE_STACK_H
#define TE_STATE_STACK_H

#include "runnable.h"
#include "game_state.h"

#include <vector>
#include <memory>

namespace te
{
	class Application;

	class StateFactory
	{
	public:
		StateFactory(StateStack&);

		template <typename State, typename... Args>
		std::unique_ptr<State> makeState(Args&&... args) const
		{
			return std::unique_ptr<State>{new State{mStack, std::forward<Args>(args)...}};
		}
	private:
		StateStack& mStack;
	};

	class StateStack : public Runnable
	{
	public:
		template<typename T, typename... Args>
		static std::unique_ptr<StateStack> make(Application& app, Args&&... args)
		{
			std::unique_ptr<StateStack> pStack{new StateStack{app}};
			pStack->mStack.push_back(pStack->mStateFactory.makeState<T>(std::forward<Args>(args)...));
			return pStack;
		}

		void processInput(const sf::Event&);
		void update(const sf::Time&);

	private:
		friend class GameState;

		StateStack(Application& app);

		void queuePush(std::unique_ptr<GameState>&&);
		void queuePop();
		void queueClear();

		void processPendingActions();

		enum class ActionType { Push, Pop, Clear };
		struct Action
		{
			ActionType type;
			std::unique_ptr<GameState> pState;
		};

		void draw(sf::RenderTarget&, sf::RenderStates) const;
		Application& getApplication();
		const StateFactory& getStateFactory() const;

		Application& mApp;
		StateFactory mStateFactory;
		std::vector<std::unique_ptr<GameState>> mStack;
		std::vector<Action> mPendingActions;
	};
}

#endif
