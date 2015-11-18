#include <wrappers.h>
#include <tmx.h>
#include <game_state.h>
#include <tiled_map.h>
#include <shader.h>

#include <lua.hpp>
#include <LuaBridge.h>
#include <glm/gtx/transform.hpp>

#include <functional>
#include <iostream>
#include <algorithm>

namespace te
{
    class LuaGameState : public GameState
    {
    public:
        LuaGameState(TMX&& tmx, Shader&& shader)
            : mpTMX(new TMX(std::move(tmx)))
            , mpShader(new Shader(std::move(shader)))
            , mpTiledMap(new TiledMap(mpTMX, mpShader))
            , mpL(
                luaL_newstate(),
                [](lua_State* L) { lua_close(L); })
        {
            init();
        }
        LuaGameState(std::shared_ptr<TMX> pTMX, std::shared_ptr<Shader> pShader)
            : mpTMX(pTMX)
            , mpShader(pShader)
            , mpTiledMap(new TiledMap(mpTMX, mpShader))
            , mpL(
                luaL_newstate(),
                [](lua_State* L) { lua_close(L); })
        {
            init();
        }

        bool processInput(const SDL_Event&) { return false; }
        bool update(float dt) { return false; }
        void draw()
        {
            mpTiledMap->draw();
        }

    private:
        void init()
        {
            lua_State* L = mpL.get();

            luaL_openlibs(L);

            int status = luaL_dofile(L, (mpTMX->meta.path + "/main.lua").c_str());
            if (status) {
                throw std::runtime_error("LuaGameState::init: Could not load main.lua.");
            }

            luabridge::LuaRef mainRef(luabridge::getGlobal(L, "main"));
            if (mainRef.isNil()) {
                throw std::runtime_error("LuaGameState::init: Could not find main function.");
            }

            luabridge::getGlobalNamespace(L)
                .beginNamespace("te")
                .endNamespace();
        }

        std::shared_ptr<TMX> mpTMX;
        std::shared_ptr<Shader> mpShader;
        std::shared_ptr<TiledMap> mpTiledMap;
        std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;
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

        te::WindowPtr pWindow = te::createWindowOpenGL(
            "Map Runner",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN);

        std::shared_ptr<te::LuaGameState> pState(new te::LuaGameState(
            te::TMX(argv[1]),
            te::Shader(glm::ortho<GLfloat>(0, (GLfloat)WINDOW_WIDTH, (GLfloat)WINDOW_HEIGHT, 0, -100, 100), glm::mat4())));
        te::StateStack stateStack(pState);
        te::executeStack(stateStack, *pWindow);

    } catch (std::exception ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
