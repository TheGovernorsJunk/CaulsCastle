#include "game_state.h"
#include <algorithm>
#include <SDL_events.h>

namespace te
{
    static void throwNoStackException()
    {
        throw std::runtime_error("State not associated with stack.");
    }

    GameState::GameState()
        : mpStack(nullptr)
    {}
    GameState::~GameState() {}

    void GameState::queuePop()
    {
        if (mpStack) {
            mpStack->mPendingChanges.push_back({
                StackOp::POP,
                nullptr,
                *this
            });
        } else {
            throwNoStackException();
        }
    }

    void GameState::queuePush(std::shared_ptr<GameState> newState)
    {
        if (mpStack) {
            mpStack->mPendingChanges.push_back({
                StackOp::PUSH,
                newState,
                *this
            });
        } else {
            throwNoStackException();
        }
    }

    void GameState::queueClear()
    {
        if (mpStack) {
            mpStack->mPendingChanges.push_back({
                StackOp::CLEAR,
                nullptr,
                *this
            });
        } else {
            throwNoStackException();
        }
    }

    void StateStack::applyPendingChanges()
    {
        std::for_each(std::begin(mPendingChanges), std::end(mPendingChanges), [this](GameState::Change& change)
        {
            switch (change.op) {
            case GameState::StackOp::PUSH:
                this->push(change.state);
                break;
            case GameState::StackOp::POP:
                this->popAt(change.issuer);
                break;
            case GameState::StackOp::CLEAR:
                this->clear();
                break;
            default:
                throw std::runtime_error("Unsupported stack operation.");
            }
        });
        mPendingChanges.clear();
    }

    StateStack::StateStack(std::shared_ptr<GameState> pInitialState)
        : mStack()
    {
        push(pInitialState);
    }

    void StateStack::push(std::shared_ptr<GameState> pState)
    {
        if (pState) {
            if (!pState->mpStack) {
                pState->mpStack = this;
                mStack.push_back(pState);
            }
            else {
                throw std::runtime_error("State already in a stack.");
            }
        }
        else {
            throw std::runtime_error("Must supply state to stack.");
        }
    }

    void StateStack::popAt(GameState& state)
    {
        auto stateIt = mStack.end();
        for (auto it = mStack.begin(); it != mStack.end(); ++it) {
            if (it->get() == &state) {
                stateIt = it;
                break;
            }
        }
        mStack.erase(stateIt, mStack.end());
    }

    void StateStack::clear()
    {
        mStack.clear();
    }

    void StateStack::processInput(const SDL_Event& e)
    {
        for (auto it = mStack.rbegin(); it != mStack.rend(); ++it) {
            if (!it->get()->processInput(e)) {
                break;
            }
        }
    }

    void StateStack::update(float dt)
    {
        for (auto it = mStack.rbegin(); it != mStack.rend(); ++it) {
            if (!it->get()->update(dt)) {
                break;
            }
        }
        applyPendingChanges();
    }

    void StateStack::draw() const
    {
        for (auto it = mStack.begin(); it != mStack.end(); ++it) {
            it->get()->draw();
        }
    }

    void executeStack(StateStack& stack, const std::vector<const SDL_Event>& events, float dt)
    {
        std::for_each(std::begin(events), std::end(events), [&stack](const SDL_Event& evt)
        {
            stack.processInput(evt);
        });
        stack.update(dt);
        stack.draw();
    }
}
