#include "game_state.h"
#include <algorithm>
#include <SDL.h>
#include "gl.h"

namespace te
{
    NoStackException::NoStackException()
        : std::runtime_error("State not associated with stack.") {}
    NullptrStateException::NullptrStateException()
        : std::runtime_error("Must supply state to stack.") {}
    BusyStateException::BusyStateException()
        : std::runtime_error("State already in a stack.") {}

    GameState::GameState()
        : mpStack(nullptr)
    {}
    GameState::~GameState() {}

    bool GameState::inStack()
    {
        return mpStack != nullptr;
    }

    void GameState::queuePop()
    {
        if (mpStack) {
            mpStack->mPendingChanges.push_back({
                StackOp::POP,
                nullptr,
                *this
            });
        } else {
            throw NoStackException();
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
            throw NoStackException();
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
            throw NoStackException();
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
            if (pState->mpStack == nullptr) {
                pState->mpStack = this;
                mStack.push_back(pState);
            }
            else {
                mPendingChanges.clear();
                throw BusyStateException();
            }
        }
        else {
            mPendingChanges.clear();
            throw NullptrStateException();
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

        for (auto nullifyIt = stateIt; nullifyIt != mStack.end(); ++nullifyIt) {
            (*nullifyIt)->mpStack = nullptr;
        }

        mStack.erase(stateIt, mStack.end());
    }

    void StateStack::clear()
    {
        std::for_each(std::begin(mStack), std::end(mStack), [](std::shared_ptr<GameState>& pState) {
            pState->mpStack = nullptr;
        });
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

    bool StateStack::empty() const
    {
        return mStack.empty();
    }

    void tickStack(StateStack& stack, const std::vector<const SDL_Event>& events, float dt)
    {
        std::for_each(std::begin(events), std::end(events), [&stack](const SDL_Event& evt)
        {
            stack.processInput(evt);
        });
        stack.update(dt);
        stack.draw();
    }

    void executeStack(StateStack& stack, SDL_Window& window)
    {
        std::vector<const SDL_Event> events;
        SDL_Event e;

        bool running = true;

        Uint64 t0 = SDL_GetPerformanceCounter();

        while (!stack.empty() && running == true)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    running = false;
                }
                events.push_back(e);
            }

            Uint64 now = SDL_GetPerformanceCounter();
            float dt = (float)(now - t0) / SDL_GetPerformanceFrequency();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            tickStack(stack, events, dt);
            events.clear();

            SDL_GL_SwapWindow(&window);
            t0 = now;
        }
    }
}
