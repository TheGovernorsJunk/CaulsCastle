#include "lua_game_state.h"

#include "tmx.h"
#include "shader.h"
#include "tiled_map.h"

#include <glm/gtx/transform.hpp>

#include <iostream>

namespace te
{
    LuaGameState::LuaGameState(std::shared_ptr<TMX> pTMX)
        : LuaGameState(pTMX, AssetManager(pTMX))
    {}
    LuaGameState::LuaGameState(std::shared_ptr<TMX> pTMX, const AssetManager& assets)
        : mpShader(new Shader(glm::ortho<GLfloat>(0, 16, 9, 0, -100, 100)))
        , mAssets(assets)
        , mECS(mpShader)
        , mLuaStateECS(mECS)
        , mpTiledMap(nullptr)
    {
        glm::mat4 model = glm::scale(glm::vec3(1.f / pTMX->tilewidth, 1.f / pTMX->tileheight, 1.f));
        mpTiledMap = std::shared_ptr<TiledMap>(new TiledMap(pTMX, mpShader, model, assets.pTextureManager.get()));
        loadObjects(*pTMX, model, mAssets, mECS);
        try {
            mLuaStateECS.loadScript(pTMX->meta.path + "/main.lua");
            mLuaStateECS.runScript();
        } catch (const std::runtime_error&) {
            std::clog << "Warning: LuaGameState: Could not load main.lua" << std::endl;
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
        mpTiledMap->draw();
        te::draw(mECS);
    }

    void LuaGameState::runConsole()
    {
        mLuaStateECS.runConsole();
    }
}
