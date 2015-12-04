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
        LuaGameState(std::shared_ptr<const TMX>, const glm::mat4& projection, const glm::mat4& model);
        LuaGameState(std::shared_ptr<const TMX>, const glm::mat4& projection, const glm::mat4& model, const AssetManager&);
        LuaGameState(std::shared_ptr<const TMX>, std::shared_ptr<const Shader> pShader, const glm::mat4& model);
        LuaGameState(std::shared_ptr<const TMX>, std::shared_ptr<const Shader> pShader, const glm::mat4& model, const AssetManager&);

        bool processInput(const SDL_Event&);
        bool update(float dt);
        void draw();

        void runConsole();

    private:
        std::shared_ptr<const Shader> mpShader;
        AssetManager mAssets;
        std::shared_ptr<TiledMap> mpTiledMap;
        ECS mECS;
        ECSWatchers mECSWatchers;
        LuaStateECS mLuaStateECS;
    };
}

#endif
