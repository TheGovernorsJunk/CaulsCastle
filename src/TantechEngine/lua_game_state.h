#ifndef TE_LUA_GAME_STATE
#define TE_LUA_GAME_STATE

#include "game_state.h"
#include "ecs.h"

#include <memory>

namespace te
{
    struct TMX;
    class Shader;
    class TiledMap;

    class LuaGameState : public GameState {
    public:
        LuaGameState(std::shared_ptr<TMX>, const glm::mat4& projection);
        LuaGameState(std::shared_ptr<TMX>, const glm::mat4& projection, const AssetManager&);

        bool processInput(const SDL_Event&);
        bool update(float dt);
        void draw();

        void runConsole();

    private:
        std::shared_ptr<Shader> mpShader;
        AssetManager mAssets;
        ECS mECS;
        LuaStateECS mLuaStateECS;
        std::shared_ptr<TiledMap> mpTiledMap;
    };
}

#endif
