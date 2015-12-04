#include "lua_game_state.h"

#include "tmx.h"
#include "shader.h"
#include "tiled_map.h"
#include "camera.h"

#include <glm/gtx/transform.hpp>

#include <iostream>
#include <cassert>

namespace te
{
    LuaGameState::LuaGameState(std::shared_ptr<const TMX> pTMX, const glm::mat4& projection, const glm::mat4& model)
        : LuaGameState(pTMX, std::shared_ptr<const Shader>(new Shader(projection)), model, AssetManager(pTMX))
    {}
    LuaGameState::LuaGameState(std::shared_ptr<const TMX> pTMX, const glm::mat4& projection, const glm::mat4& model, const AssetManager& assets)
        : LuaGameState(pTMX, std::shared_ptr<const Shader>(new Shader(projection)), model, assets)
    {}
    LuaGameState::LuaGameState(std::shared_ptr<const TMX> pTMX, std::shared_ptr<const Shader> pShader, const glm::mat4& model)
        : LuaGameState(pTMX, pShader, model, AssetManager(pTMX))
    {}
    LuaGameState::LuaGameState(std::shared_ptr<const TMX> pTMX, std::shared_ptr<const Shader> pShader, const glm::mat4& model, const AssetManager& assets)
        : mpShader(pShader)
        , mAssets(assets)
        , mpTiledMap(new TiledMap(pTMX, pShader, model, assets.pTextureManager.get()))
        , mECS()
        , mECSWatchers(mECS, pShader)
        , mLuaStateECS(mECS, mECSWatchers)
    {
        assert(pTMX && pShader);

        loadObjects(*pTMX, model, mAssets, mECS);
        try {
            mLuaStateECS.loadScript(pTMX->meta.path + "/main.lua");
            mLuaStateECS.runScript();
        } catch (const std::runtime_error& ex) {
            std::clog << "Warning: LuaGameState: Could not load main.lua." << std::endl;
            std::clog << ex.what() << std::endl;
        }
    }

    bool LuaGameState::processInput(const SDL_Event&) { return false; }
    bool LuaGameState::update(float dt)
    {
        te::update(mECSWatchers, dt);
        return false;
    }
    void LuaGameState::draw()
    {
        mpTiledMap->draw(mECSWatchers.pCamera->getView());
        te::draw(mECSWatchers);
    }

    void LuaGameState::runConsole()
    {
        mLuaStateECS.runConsole();
    }
}
