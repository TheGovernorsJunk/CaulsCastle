#include "lua_game_state.h"

#include "tmx.h"
#include "shader.h"
#include "tiled_map.h"
#include "transform_component.h"
#include "entity_manager.h"
#include "texture_manager.h"
#include "mesh_manager.h"
#include "animation_component.h"
#include "data_component.h"
#include "render_system.h"
#include "ecs.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <glm/gtx/transform.hpp>

#include <functional>
#include <iostream>

namespace te
{
    struct LuaGameState::Impl {

        std::unique_ptr<lua_State, std::function<void(lua_State*)>> pL;
        std::shared_ptr<Shader> pShader;

        std::shared_ptr<TiledMap> pTiledMap;

        AssetManager assets;
        ECS ecs;


        Entity getEntity(unsigned tiledId)
        {
            return ecs.pDataComponent->getEntity(tiledId);
        }
        glm::mat4 translatef(const Entity& entity, float x, float y, float z)
        {
            return ecs.pTransformComponent->multiplyTransform(entity, glm::translate(glm::vec3(x, y, z)));
        }
        glm::mat4 translateWorldf(const Entity& entity, float x, float y, float z)
        {
            return ecs.pTransformComponent->multiplyTransform(entity, glm::translate(glm::vec3(x, y, z)), TransformComponent::Space::WORLD);
        }
        glm::mat4 translatev(const Entity& entity, glm::vec3 translation)
        {
            return ecs.pTransformComponent->multiplyTransform(entity, glm::translate(translation));
        }
        glm::mat4 translateWorldv(const Entity& entity, glm::vec3 translation)
        {
            return ecs.pTransformComponent->multiplyTransform(entity, glm::translate(translation), TransformComponent::Space::WORLD);
        }
        glm::mat4 scalef(const Entity& entity, float x, float y, float z)
        {
            return ecs.pTransformComponent->multiplyTransform(entity, glm::scale(glm::vec3(x, y, z)));
        }
        glm::mat4 scalev(const Entity& entity, glm::vec3 scale)
        {
            return ecs.pTransformComponent->multiplyTransform(entity, glm::scale(scale));
        }

        void printEntities()
        {
            ecs.pDataComponent->forEach([](const Entity& entity, const DataInstance& instance) {
                std::cout << instance.id << ": " << instance.name << std::endl;
            });
        }

        void destroyEntity(const Entity& entity)
        {
            ecs.pEntityManager->destroy(entity);
        }

        Impl(std::shared_ptr<const TMX> pTMX, const AssetManager& _assets)

            : pL(luaL_newstate(),
                 [](lua_State* L) { lua_close(L); })
            , pShader(new Shader(glm::ortho<GLfloat>(0, 16, 9, 0, -100, 100)))

            , pTiledMap(nullptr)

            , assets(_assets)
            , ecs(pShader)

        {
            glm::mat4 model = glm::scale(glm::vec3(1.f / pTMX->tilewidth, 1.f / pTMX->tileheight, 1.f));
            pTiledMap = std::shared_ptr<TiledMap>(new TiledMap(pTMX, pShader, model, assets.pTextureManager.get()));
            te::loadObjects(*pTMX, model, assets, ecs);

            lua_State* L = pL.get();
            luaL_openlibs(L);
            luabridge::getGlobalNamespace(L)
                .beginNamespace("tt")

                    .beginClass<Entity>("Entity")
                    .endClass()

                    .beginClass<glm::vec3>("vec3")
                        .addConstructor<void(*)(float, float, float)>()
                    .endClass()
                    .beginClass<glm::mat4>("mat4")
                    .endClass()

                    .beginClass<Impl>("State")
                        .addFunction("getEntity", &Impl::getEntity)
                        .addFunction("translatef", &Impl::translatef)
                        .addFunction("translateWorldf", &Impl::translateWorldf)
                        .addFunction("translatev", &Impl::translatev)
                        .addFunction("translateWorldv", &Impl::translateWorldv)
                        .addFunction("scalef", &Impl::scalef)
                        .addFunction("scalev", &Impl::scalev)
                        .addFunction("printEntities", &Impl::printEntities)
                    .endClass()
                .endNamespace();

            luabridge::push(L, this);
            lua_setglobal(L, "te");

            int status = luaL_dofile(L, (pTMX->meta.path + "/main.lua").c_str());
            if (status) {
                // throw std::runtime_error("LuaGameState::init: Could not load main.lua.");
                std::clog << "Warning: LuaGameState: Could not load main.lua." << std::endl;
            } else {

                luabridge::LuaRef mainRef(luabridge::getGlobal(L, "main"));
                if (mainRef.isNil()) {
                    throw std::runtime_error("LuaGameState::init: Could not find main function.");
                }

                mainRef();
            }
        }
    };

    LuaGameState::LuaGameState(std::shared_ptr<TMX> pTMX)
        : LuaGameState(pTMX, AssetManager(pTMX))
    {}
    LuaGameState::LuaGameState(std::shared_ptr<TMX> pTMX, const AssetManager& assets)
        : mpImpl(new Impl(pTMX, assets))
    {}

    bool LuaGameState::processInput(const SDL_Event&) { return false; }
    bool LuaGameState::update(float dt)
    {
        mpImpl->ecs.pRenderSystem->update(dt);
        return false;
    }
    void LuaGameState::draw()
    {
        mpImpl->pTiledMap->draw();
        mpImpl->ecs.pRenderSystem->draw();
    }

    void LuaGameState::runConsole()
    {
        lua_State* L = mpImpl->pL.get();
        int status = luaL_dofile(L, "assets/lua/console.lua");
        if (status) {
            throw std::runtime_error("LuaGameState::runConsole: Could not load console.lua");
        }
        luabridge::LuaRef mainRef(luabridge::getGlobal(L, "main"));
        if (mainRef.isNil()) {
            throw std::runtime_error("LuaGameState::runConsole: Could not load console.lua main function.");
        }

        while (true) {
            try {
                mainRef();
            }
            catch (const luabridge::LuaException& ex) {
                std::cerr << ex.what() << std::endl;
            }
        }
    }
}
