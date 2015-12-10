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
        LuaGameState(const std::shared_ptr<const TMX>&, const std::shared_ptr<Shader>& pShader, const glm::mat4& model);
        LuaGameState(const std::shared_ptr<const TMX>&, const std::shared_ptr<Shader>& pShader, const glm::mat4& model, const AssetManager&);

        bool processInput(const SDL_Event&);
        bool update(float dt);
        void draw();

        void runConsole();

    private:
        AssetManager mAssets;
        std::shared_ptr<TiledMap> mpTiledMap;
        ECS mECS;
        ECSWatchers mECSWatchers;
        LuaStateECS mLuaStateECS;
    };
}

#endif
