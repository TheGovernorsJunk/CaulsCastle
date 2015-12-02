#ifndef TE_LUA_GAME_STATE
#define TE_LUA_GAME_STATE

#include "game_state.h"

#include <memory>

namespace te
{
    struct TMX;
    class TextureManager;
    class MeshManager;
    struct AssetManager;

    class LuaGameState : public GameState {
    public:
        LuaGameState(TMX&&);
        LuaGameState(std::shared_ptr<TMX>);
        LuaGameState(TMX&&, const AssetManager&);
        LuaGameState(std::shared_ptr<TMX>, const AssetManager&);

        bool processInput(const SDL_Event&);
        bool update(float dt);
        void draw();

        void runConsole();

    private:
        struct Impl;
        std::unique_ptr<Impl> mpImpl;
    };
}

#endif
