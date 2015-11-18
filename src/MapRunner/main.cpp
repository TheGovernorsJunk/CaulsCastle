#include <wrappers.h>
#include <tmx.h>
#include <game_state.h>

#include <string.h>
#include <iostream>
#include <algorithm>

namespace te
{
    class LuaGameState : public GameState
    {
    public:
        bool processInput(const SDL_Event&) { return false; }
        bool update(float dt) { return false; }
        void draw() {}
    };
}

int main(int argc, char* argv[])
{
    static const int WINDOW_WIDTH = 1024;
    static const int WINDOW_HEIGHT = 768;

    if (argc != 2) {
        std::cerr << "Incorrect usage: Must supply path to Tiled export Lua file." << std::endl;
        return -1;
    }

    try {

        const te::Initialization init;

        const te::TMX tmx(argv[1]);

        te::WindowPtr pWindow = te::createWindowOpenGL(
            "Map Runner",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN);

        std::shared_ptr<te::LuaGameState> pState(new te::LuaGameState());
        te::StateStack stateStack(pState);
        te::executeStack(stateStack, *pWindow);

    } catch (std::exception ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
