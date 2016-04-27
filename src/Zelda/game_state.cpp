#include "game_state.h"
#include "state_stack.h"
#include "application.h"

#include <cassert>

namespace te
{
	void GameState::popState()
	{
		assert(mStateStack);
		mStateStack->queuePop();
	}

	void GameState::clearStates()
	{
		assert(mStateStack);
		mStateStack->queueClear();
	}

	TextureManager& GameState::getTextureManager()
	{
		assert(mStateStack);
		return mStateStack->getApplication().getTextureManager();
	}
}
