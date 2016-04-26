#include "game_state.h"
#include "state_stack.h"

namespace te
{
	GameState::GameState(StateStack& stack)
		: mStateStack(stack) {}

	void GameState::popState()
	{
		mStateStack.queuePop();
	}

	void GameState::clearStates()
	{
		mStateStack.queueClear();
	}
}
