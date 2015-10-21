#ifndef TE_GAME_STATE
#define TE_GAME_STATE

#include <memory>
#include <deque>
#include <vector>

class StateStack;

class GameState
{
public:
    GameState();
    virtual ~GameState();

protected:
    void queuePop();
    void queuePush(std::shared_ptr<GameState> newState);
    void queueClear();
    void applyPendingChanges();

private:
    friend class StateStack;

    virtual bool update(float) = 0;
    virtual bool draw() = 0;

    GameState(const GameState&) = delete;
    GameState& operator=(const GameState&) = delete;

    enum class StackOp {
        PUSH,
        POP,
        CLEAR
    };
    struct Change {
        StackOp op;
        std::shared_ptr<GameState> state;
    };

    StateStack *mpStack;
    std::deque<Change> mPendingChanges;
};

class StateStack
{
public:
    StateStack(std::shared_ptr<GameState> pInitialState);

    void update(float dt) const;
    void draw() const;

private:
    friend class GameState;

    void push(std::shared_ptr<GameState> pState);
    void popAt(GameState* pState);
    void clear();

    StateStack(const StateStack&) = delete;
    StateStack& operator=(const StateStack&) = delete;

    std::vector<std::shared_ptr<GameState>> mStack;
};

#endif
