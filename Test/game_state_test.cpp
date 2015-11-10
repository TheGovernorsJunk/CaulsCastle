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

    TEST_F(StateStackTest, Exception) {
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
        // Ensure clearing on throw
        pState->pushState(nullptr);
        EXPECT_THROW(ss.update(0), NullptrStateException);

        // No stack after pop
        std::shared_ptr<TestGameState> pState2(new TestGameState());
        pState->pushState(pState2);
        pState->pop();
        ss.update(0);
        EXPECT_THROW(pState->pushState(pState), NoStackException) << "Can't request push without stack";
        EXPECT_THROW(pState->pop(), NoStackException) << "Can't request pop without stack";
        EXPECT_THROW(pState->clear(), NoStackException) << "Can't request clear without stack";
        EXPECT_THROW(pState2->pushState(pState), NoStackException) << "Can't request push without stack";
        EXPECT_THROW(pState2->pop(), NoStackException) << "Can't request pop without stack";
        EXPECT_THROW(pState2->clear(), NoStackException) << "Can't request clear without stack";
    }

    TEST_F(StateStackTest, BasicOps) {
        StateStack ss(pState);
        EXPECT_EQ(false, ss.empty());
        EXPECT_EQ(true, pState->inStack());
        pState->pop();
        EXPECT_EQ(false, ss.empty()) << "State shouldn't pop until stack updates";
        EXPECT_EQ(true, pState->inStack());
        ss.update(0);
        EXPECT_EQ(true, ss.empty()) << "Stack should be empty after updating";
        EXPECT_EQ(false, pState->inStack());

        StateStack ss2(pState);
        pState->clear();
        ss2.update(0);
        EXPECT_EQ(true, ss.empty());
        EXPECT_EQ(false, pState->inStack());
    }

    TEST_F(StateStackTest, MultipleOps) {
        StateStack ss(pState);
        // Fine to queue push after queue pop
        pState->pop();
        pState->pushState(pState);
        ss.update(0);
        EXPECT_EQ(false, ss.empty());
        EXPECT_EQ(true, pState->inStack());
    }
}
