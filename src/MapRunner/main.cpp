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
#include <view.h>

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

        te::View view({ 0, 0, 16, 9 });
        view.setViewport({ 0.f, 0.f, 1.f, 1.f });
        auto pShader = std::make_shared<te::Shader>(view, *pWindow);

        auto pTMX = std::make_shared<te::TMX>(argv[1]);
        auto pState = std::make_shared<te::LuaGameState>(
            pTMX,
            pShader,
            glm::scale(glm::vec3(1.f/pTMX->tilewidth, 1.f/pTMX->tileheight, 1.f)));
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
