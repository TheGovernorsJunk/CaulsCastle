#include <wrappers.h>
#include <tmx.h>
#include <game_state.h>
#include <texture_manager.h>
#include <mesh_manager.h>
#include <animation_factory.h>
#include <tiled_map.h>
#include <shader.h>
#include <entity_manager.h>
#include <transform_component.h>
#include <animation_component.h>
#include <data_component.h>
#include <render_system.h>
#include <lua_game_state.h>
#include <ecs.h>

#include <lua.hpp>
#include <LuaBridge.h>
#include <glm/gtx/transform.hpp>

#include <functional>
#include <iostream>
#include <algorithm>
#include <thread>

int main(int argc, char* argv[])
{
    static const int WINDOW_WIDTH = 1280;
    static const int WINDOW_HEIGHT = 720;

    if (argc != 2) {
        std::cerr << "Incorrect usage: Must supply path to Tiled export Lua file." << std::endl;
        return -1;
    }

    try {

        const te::Initialization init;

        te::WindowPtr pWindow = te::createWindowOpenGL(
            "Map Runner",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN);

        std::shared_ptr<te::LuaGameState> pState(new te::LuaGameState(
            std::shared_ptr<te::TMX>(new te::TMX(argv[1]))));
        te::StateStack stateStack(pState);

        bool running = true;

        std::thread prompt([&running, pState] {
            pState->runConsole();
        });

        te::executeStack(stateStack, *pWindow, &running);
        prompt.join();

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
