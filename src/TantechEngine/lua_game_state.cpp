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
        , mpTiledMap(new TiledMap(pTMX, mpShader, model, assets.pTextureManager.get()))
        , mECS(pShader)
        , mLuaStateECS(mECS)
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
        te::update(mECS, dt);
        return false;
    }
    void LuaGameState::draw()
    {
        mpTiledMap->draw(mECS.pCamera->getView());
        te::draw(mECS);
    }

    void LuaGameState::runConsole()
    {
        mLuaStateECS.runConsole();
    }
}
