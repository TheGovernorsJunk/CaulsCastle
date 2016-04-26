#include "game_state.h"
#include "state_stack.h"

namespace te
{
	void GameState::popState()
	{
		mStateStack->queuePop();
	}

	void GameState::clearStates()
	{
		mStateStack->queueClear();
	}
}
