#include "game_state.h"
#include <algorithm>

GameState::GameState(std::shared_ptr<StateStack> pStack)
	: mpStack(pStack)
{}
GameState::~GameState() {}

void GameState::queuePop()
{
	mPendingChanges.push_back({
		StackOp::POP,
		nullptr
	});
}

void GameState::queuePush(std::shared_ptr<GameState> newState)
{
	mPendingChanges.push_back({
		StackOp::PUSH,
		newState
	});
}

void GameState::queueClear()
{
	mPendingChanges.push_back({
		StackOp::CLEAR,
		nullptr
	});
}

void GameState::applyPendingChanges()
{
	StateStack& stack = *mpStack;
	std::for_each(std::begin(mPendingChanges), std::end(mPendingChanges), [&stack, this](Change& change)
	{
		switch (change.op) {
		case StackOp::PUSH:
			stack.push(change.state);
			break;
		case StackOp::POP:
			stack.popAt(this);
			break;
		case StackOp::CLEAR:
			stack.clear();
			break;
		default:
			throw std::runtime_error("Unsupported stack operation.");
		}
	});
	mPendingChanges.clear();
}

void StateStack::push(std::shared_ptr<GameState> pState)
{
	mStack.push_back(pState);
}

void StateStack::popAt(GameState* pState)
{
	auto last = mStack.begin();
	for (auto it = mStack.begin(); it != mStack.end(); ++it) {
		if (it->get() == pState) {
			last = it + 1;
			continue;
		}
	}
	mStack.erase(mStack.begin(), last);
}

void StateStack::clear()
{
	mStack.clear();
}

void StateStack::update(float dt)
{
	for (auto it = mStack.begin(); it != mStack.end(); ++it) {
		if (!it->get()->update(dt)) {
			continue;
		}
	}
}
