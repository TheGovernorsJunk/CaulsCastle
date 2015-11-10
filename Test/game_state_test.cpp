#include <game_state.h>

#include <gtest/gtest.h>

#include <memory>

namespace te
{
    class TestGameState : public GameState {
    public:
        void pushState(std::shared_ptr<GameState> ptr) {
            queuePush(ptr);
        }
        void pop() {
            queuePop();
        }
        void clear() {
            queueClear();
        }

        bool processInput(const SDL_Event&) { return true; }
        bool update(float) { return true; }
        void draw() {}
    };

    class StateStackTest : public ::testing::Test {
    public:
        std::shared_ptr<TestGameState> pState;
        StateStackTest() : pState(new TestGameState()) {}
    };

    TEST_F(StateStackTest, Death) {
        // Bad initialization
        EXPECT_THROW(StateStack(nullptr), NullptrStateException) << "Should not accept nullptr";

        // Premature stack ops
        EXPECT_THROW(pState->pushState(pState), NoStackException) << "Can't request push without stack";
        EXPECT_THROW(pState->pop(), NoStackException) << "Can't request pop without stack";
        EXPECT_THROW(pState->clear(), NoStackException) << "Can't request clear without stack";

        StateStack ss(pState);
        pState->pushState(nullptr);
        EXPECT_THROW(ss.update(0), NullptrStateException);
        pState->pushState(pState);
        EXPECT_THROW(ss.update(0), BusyStateException);
    }

    TEST_F(StateStackTest, Ops) {
        StateStack ss(pState);
        EXPECT_EQ(false, ss.empty());
        pState->pop();
        EXPECT_EQ(false, ss.empty()) << "State shouldn't pop until stack updates";
        ss.update(0);
        EXPECT_EQ(true, ss.empty()) << "Stack should be empty after updating";
    }
}
