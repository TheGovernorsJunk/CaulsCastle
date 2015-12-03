#include "ecs.h"

#include "entity_manager.h"
#include "data_component.h"
#include "transform_component.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <functional>
#include <iostream>

namespace te
{
    struct LuaStateECS::Impl {
        // members
        std::unique_ptr<lua_State, std::function<void(lua_State*)>> pL;
        ECS ecs;
        luabridge::LuaRef mainRef;

        // methods for Lua
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

        // constructors
        Impl(const ECS& ecs)
            : pL(luaL_newstate(),
                 [](lua_State* L) { lua_close(L); })
            , ecs(ecs)
            , mainRef(luabridge::LuaRef(pL.get()))
        {
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
                        .addFunction("destroyEntity", &Impl::destroyEntity)
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
        }
    };

    LuaStateECS::LuaStateECS(const ECS& ecs)
        : mpImpl(new Impl(ecs)) {}
    LuaStateECS::~LuaStateECS()
    {
        delete mpImpl;
    }

    void LuaStateECS::loadScript(const std::string& path) const
    {
        lua_State* L = mpImpl->pL.get();
        int status = luaL_dofile(L, path.c_str());
        if (status) {
            throw std::runtime_error("LuaStateECS::loadScript: Could not load Lua script.");
        }

        luabridge::LuaRef mainRef(luabridge::getGlobal(L, "main"));
        if (mainRef.isNil() || !mainRef.isFunction()) {
            throw std::runtime_error("LuaStateECS::loadScript: Could not find main function.");
        }

        mpImpl->mainRef = mainRef;
    }

    void LuaStateECS::runScript() const
    {
        luabridge::LuaRef& mainRef = mpImpl->mainRef;
        if (!mainRef.isNil()) {
            mainRef();
        } else {
            throw std::runtime_error("LuaStateECS::runScript: No Lua script loaded.");
        }
    }

    void LuaStateECS::runConsole() const
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
