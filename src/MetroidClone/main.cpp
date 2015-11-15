#include <game_state.h>
#include <wrappers.h>

#include <memory>

namespace te
{
    class MyState : public GameState
    {
    public:
        bool processInput(const SDL_Event&) { return true; }
        bool update(float dt) { return true; }
        void draw() {}
    };
}

int main(int argc, char* argv[])
{
    te::Initialization init;

    te::WindowPtr pWindow = te::createWindowOpenGL(
        "Metroid Clone",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        720,
        480,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    std::shared_ptr<te::MyState> myState(new te::MyState());
    te::StateStack ss(myState);
    te::executeStack(ss, *pWindow);

    return 0;
}
