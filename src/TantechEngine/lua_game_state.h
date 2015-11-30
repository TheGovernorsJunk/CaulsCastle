#ifndef TE_LUA_GAME_STATE
#define TE_LUA_GAME_STATE

#include "game_state.h"

#include <memory>

namespace te
{
    struct TMX;
    class TextureManager;
    class MeshManager;

    class LuaGameState : public GameState {
    public:
        LuaGameState(
            TMX&&,
            std::shared_ptr<TextureManager> = std::shared_ptr<TextureManager>(nullptr),
            std::shared_ptr<MeshManager> = std::shared_ptr<MeshManager>(nullptr));
        LuaGameState(
            std::shared_ptr<TMX>,
            std::shared_ptr<TextureManager> = std::shared_ptr<TextureManager>(nullptr),
            std::shared_ptr<MeshManager> = std::shared_ptr<MeshManager>(nullptr));

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
