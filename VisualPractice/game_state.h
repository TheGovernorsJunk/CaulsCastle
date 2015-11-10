#ifndef TE_GAME_STATE
#define TE_GAME_STATE

#include <memory>
#include <deque>
#include <vector>

union SDL_Event;
struct SDL_Window;

namespace te
{
    class StateStack;

    class GameState
    {
    public:
        GameState();
        virtual ~GameState();

        bool inStack();

    protected:
        void queuePop();
        void queuePush(std::shared_ptr<GameState> newState);
        void queueClear();

    private:
        friend class StateStack;

        virtual bool processInput(const SDL_Event&) = 0;
        virtual bool update(float) = 0;
        virtual void draw() = 0;

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
            GameState& issuer;
        };

        StateStack *mpStack;
    };

    class StateStack
    {
    public:
        StateStack(std::shared_ptr<GameState> pInitialState);

        void processInput(const SDL_Event&);
        void update(float dt);
        void draw() const;

        bool empty() const;

    private:
        friend class GameState;

        void push(std::shared_ptr<GameState> pState);
        void popAt(GameState& pState);
        void clear();

        void applyPendingChanges();

        StateStack(const StateStack&) = delete;
        StateStack& operator=(const StateStack&) = delete;

        std::vector<std::shared_ptr<GameState>> mStack;
        std::deque<GameState::Change> mPendingChanges;
    };

    class NoStackException : public std::runtime_error {
    public:
        NoStackException();
    };
    class NullptrStateException : public std::runtime_error {
    public:
        NullptrStateException();
    };
    class BusyStateException : public std::runtime_error {
    public:
        BusyStateException();
    };

    void tickStack(StateStack&, const std::vector<const SDL_Event>& events, float dt);
    void executeStack(StateStack&, SDL_Window&);
}

#endif
