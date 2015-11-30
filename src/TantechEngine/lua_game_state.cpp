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

        std::shared_ptr<TextureManager> pTextureManager;
        std::shared_ptr<MeshManager> pMeshManager;
        std::shared_ptr<AnimationFactory> pAnimationFactory;

        std::shared_ptr<TransformComponent> pTransform;
        std::shared_ptr<AnimationComponent> pAnimation;
        std::shared_ptr<DataComponent> pData;
        std::shared_ptr<EntityManager> pEntityManager;

        std::shared_ptr<RenderSystem> pRenderSystem;


        Entity getEntity(unsigned tiledId)
        {
            return pData->getEntity(tiledId);
        }
        glm::mat4 translatef(const Entity& entity, float x, float y, float z)
        {
            return pTransform->multiplyTransform(entity, glm::translate(glm::vec3(x, y, z)));
        }
        glm::mat4 translateWorldf(const Entity& entity, float x, float y, float z)
        {
            return pTransform->multiplyTransform(entity, glm::translate(glm::vec3(x, y, z)), TransformComponent::Space::WORLD);
        }
        glm::mat4 translatev(const Entity& entity, glm::vec3 translation)
        {
            return pTransform->multiplyTransform(entity, glm::translate(translation));
        }
        glm::mat4 translateWorldv(const Entity& entity, glm::vec3 translation)
        {
            return pTransform->multiplyTransform(entity, glm::translate(translation), TransformComponent::Space::WORLD);
        }
        glm::mat4 scalef(const Entity& entity, float x, float y, float z)
        {
            return pTransform->multiplyTransform(entity, glm::scale(glm::vec3(x, y, z)));
        }
        glm::mat4 scalev(const Entity& entity, glm::vec3 scale)
        {
            return pTransform->multiplyTransform(entity, glm::scale(scale));
        }

        void printEntities()
        {
            pData->forEach([](const Entity& entity, const DataInstance& instance) {
                std::cout << instance.id << ": " << instance.name << std::endl;
            });
        }

        Impl(
            std::shared_ptr<TMX> pTMX,
            std::shared_ptr<TextureManager> pTextureManager,
            std::shared_ptr<MeshManager> pMeshManager)

            : pL(luaL_newstate(),
                 [](lua_State* L) { lua_close(L); })
            , pShader(new Shader(glm::ortho<GLfloat>(0, 16, 9, 0, -100, 100)))

            , pTiledMap(nullptr)

            , pTextureManager(pTextureManager)
            , pMeshManager(pMeshManager)
            , pAnimationFactory(nullptr)

            , pTransform(new TransformComponent())
            , pAnimation(new AnimationComponent())
            , pData(new DataComponent())
            , pEntityManager(new EntityManager(std::vector<std::shared_ptr<Observer<DestroyEvent>>>{pTransform}))

            , pRenderSystem(new RenderSystem(pShader, nullptr, pAnimation, pTransform))

        {
            // Supply managers if none given
            if (!pTextureManager) { pTextureManager = std::shared_ptr<TextureManager>(new TextureManager()); }
            if (!pMeshManager) { pMeshManager = std::shared_ptr<MeshManager>(new MeshManager(pTMX, pTextureManager)); }
            pTiledMap = std::shared_ptr<TiledMap>(new TiledMap(pTMX, pShader, glm::scale(glm::vec3(1.f / pTMX->tilewidth, 1.f / pTMX->tileheight, 1.f)), pTextureManager.get()));
            pAnimationFactory = std::shared_ptr<AnimationFactory>(new AnimationFactory(pTMX, pMeshManager));

            te::loadObjects(pTMX, glm::scale(glm::vec3(1.f/pTMX->tilewidth, 1.f/pTMX->tileheight, 1.f)), pMeshManager, *pEntityManager, *pTransform, *pAnimation, pData.get());

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
                std::clog << "Warning: LuaGameState::init: Could not load main.lua." << std::endl;
            } else {

                luabridge::LuaRef mainRef(luabridge::getGlobal(L, "main"));
                if (mainRef.isNil()) {
                    throw std::runtime_error("LuaGameState::init: Could not find main function.");
                }

                mainRef();
            }
        }
    };

    LuaGameState::LuaGameState(
        TMX&& tmx,
        std::shared_ptr<TextureManager> pTextureManager,
        std::shared_ptr<MeshManager> pMeshManager)
        : LuaGameState(std::shared_ptr<TMX>(new TMX(std::move(tmx))), pTextureManager, pMeshManager)
    {}

    LuaGameState::LuaGameState(
        std::shared_ptr<TMX> pTMX,
        std::shared_ptr<TextureManager> pTextureManager,
        std::shared_ptr<MeshManager> pMeshManager)
        : mpImpl(new Impl(pTMX, pTextureManager, pMeshManager))
    {}

    bool LuaGameState::processInput(const SDL_Event&) { return false; }
    bool LuaGameState::update(float dt)
    {
        mpImpl->pRenderSystem->update(dt);
        return false;
    }
    void LuaGameState::draw()
    {
        mpImpl->pTiledMap->draw();
        mpImpl->pRenderSystem->draw();
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
