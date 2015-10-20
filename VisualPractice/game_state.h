#ifndef TE_GAME_STATE
#define TE_GAME_STATE

#include <memory>
#include <deque>
#include <vector>

class StateStack;

class GameState
{
public:
    GameState(std::shared_ptr<StateStack> pStack);
    virtual ~GameState();

    virtual bool update(float) = 0;
    virtual bool draw() = 0;

protected:
    void queuePop();
    void queuePush(std::shared_ptr<GameState> newState);
    void queueClear();
    void applyPendingChanges();

private:
    enum class StackOp {
        PUSH,
        POP,
        CLEAR
    };
    struct Change {
        StackOp op;
        std::shared_ptr<GameState> state;
    };

    std::shared_ptr<StateStack> mpStack;
    std::deque<Change> mPendingChanges;
};

class StateStack
{
public:
    void push(std::shared_ptr<GameState> pState);
    void popAt(GameState* pState);
    void clear();

    void update(float dt) const;
    void draw() const;

private:
    std::vector<std::shared_ptr<GameState>> mStack;
};

#endif
