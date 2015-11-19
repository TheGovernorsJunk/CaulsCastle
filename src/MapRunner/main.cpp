#include <wrappers.h>
#include <tmx.h>
#include <game_state.h>
#include <texture_manager.h>
#include <mesh_manager.h>
#include <animation_factory.h>
#include <tiled_map.h>
#include <shader.h>
#include <entity_manager.h>
#include <transform_component.h>
#include <animation_component.h>
#include <data_component.h>
#include <render_system.h>

#include <lua.hpp>
#include <LuaBridge.h>
#include <glm/gtx/transform.hpp>

#include <functional>
#include <iostream>
#include <algorithm>
#include <thread>

namespace te
{
    class LuaGameState : public GameState
    {
    public:
        LuaGameState(TMX&& tmx)
            : mpTMX(new TMX(std::move(tmx)))
            , mpShader(new Shader(
                glm::ortho<GLfloat>(0, 16, 9, 0, -100, 100),
                glm::scale(glm::vec3(1.f / mpTMX->tilewidth, 1.f / mpTMX->tileheight, 1.f))))

            , mpTextureManager(new TextureManager())
            , mpMeshManager(new MeshManager(mpTMX, mpTextureManager))
            , mpAnimationFactory(new AnimationFactory(mpTMX, mpMeshManager))

            , mpTiledMap(new TiledMap(mpTMX, mpShader, mpTextureManager.get()))

            , mpL(
                luaL_newstate(),
                [](lua_State* L) { lua_close(L); })

            , mpTransformComponent(new TransformComponent())
            , mpAnimationComponent(new AnimationComponent())
            , mpDataComponent(new DataComponent())
            , mpEntityManager(new EntityManager(std::vector<std::shared_ptr<Observer<DestroyEvent>>>{ mpTransformComponent, mpAnimationComponent, mpDataComponent }))

            , mpRenderSystem(new RenderSystem(mpShader, nullptr, mpAnimationComponent, mpTransformComponent))
        {
            init();
        }
        LuaGameState(std::shared_ptr<TMX> pTMX)
            : mpTMX(pTMX)
            , mpShader(new Shader(
                glm::ortho<GLfloat>(0, 16, 9, 0, -100, 100),
                glm::scale(glm::vec3(1.f / mpTMX->tilewidth, 1.f / mpTMX->tileheight, 1.f))))

            , mpTextureManager(new TextureManager())
            , mpMeshManager(new MeshManager(mpTMX, mpTextureManager))
            , mpAnimationFactory(new AnimationFactory(mpTMX, mpMeshManager))

            , mpTiledMap(new TiledMap(mpTMX, mpShader, mpTextureManager.get()))

            , mpL(
                luaL_newstate(),
                [](lua_State* L) { lua_close(L); })

            , mpTransformComponent(new TransformComponent())
            , mpAnimationComponent(new AnimationComponent())
            , mpDataComponent(new DataComponent())
            , mpEntityManager(new EntityManager(std::vector<std::shared_ptr<Observer<DestroyEvent>>>{ mpTransformComponent, mpAnimationComponent, mpDataComponent }))

            , mpRenderSystem(new RenderSystem(mpShader, nullptr, mpAnimationComponent, mpTransformComponent))
        {
            init();
        }

        bool processInput(const SDL_Event&) { return false; }
        bool update(float dt)
        {
            return false;
        }
        void draw()
        {
            mpTiledMap->draw();
            mpRenderSystem->draw();
        }

    private:
        void printEntities()
        {
            mpDataComponent->forEach([](const Entity& entity, const DataInstance& instance) {
                std::cout << instance.id << ": " << instance.name << std::endl;
            });
        }

        void init()
        {
            te::loadObjects(mpTMX, *mpShader, mpMeshManager, *mpEntityManager, *mpTransformComponent, *mpAnimationComponent, mpDataComponent.get());

            lua_State* L = mpL.get();

            luaL_openlibs(L);

            luabridge::getGlobalNamespace(L)
                .beginNamespace("te")

                    .beginClass<LuaGameState>("LuaGameState")
                        .addFunction("printEntities", &LuaGameState::printEntities)
                    .endClass()

                    .beginClass<EntityManager>("EntityManager")
                        .addFunction("create", &EntityManager::create)
                    .endClass()

                    .beginClass<Entity>("Entity")
                    .endClass()

                    .beginClass<TransformComponent>("TransformComponent")
                        .addFunction("get", &TransformComponent::getLocalTransform)
                        .addFunction("set", &TransformComponent::setLocalTransform)
                    .endClass()

                    .beginClass<DataComponent>("DataComponent")
                        .addFunction("getEntity", &DataComponent::getEntity)
                    .endClass()

                    .beginClass<glm::mat4>("mat4")
                        .addConstructor<void(*)(void)>()
                    .endClass()

                    .beginClass<glm::vec3>("vec3")
                        .addConstructor<void(*)(float, float, float)>()
                    .endClass()

                    .addFunction("translateMatrix", static_cast<glm::mat4 (*)(const glm::mat4&, const glm::vec3&)>(&glm::translate))
                    .addFunction("translate", static_cast<glm::mat4 (*)(const glm::vec3&)>(&glm::translate))

                .endNamespace();

            luabridge::push(L, this);
            lua_setglobal(L, "te_state");
            luabridge::push(L, mpEntityManager.get());
            lua_setglobal(L, "te_entityManager");
            luabridge::push(L, mpTransformComponent.get());
            lua_setglobal(L, "te_transform");
            luabridge::push(L, mpDataComponent.get());
            lua_setglobal(L, "te_data");

            int status = luaL_dofile(L, (mpTMX->meta.path + "/main.lua").c_str());
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

        std::shared_ptr<TMX> mpTMX;
        std::shared_ptr<Shader> mpShader;

        std::shared_ptr<TextureManager> mpTextureManager;
        std::shared_ptr<MeshManager> mpMeshManager;
        std::shared_ptr<AnimationFactory> mpAnimationFactory;

        std::shared_ptr<TiledMap> mpTiledMap;
        std::unique_ptr<lua_State, std::function<void(lua_State*)>> mpL;

        std::shared_ptr<TransformComponent> mpTransformComponent;
        std::shared_ptr<AnimationComponent> mpAnimationComponent;
        std::shared_ptr<DataComponent> mpDataComponent;
        std::shared_ptr<EntityManager> mpEntityManager;

        std::shared_ptr<RenderSystem> mpRenderSystem;
    };
}

int main(int argc, char* argv[])
{
    static const int WINDOW_WIDTH = 1280;
    static const int WINDOW_HEIGHT = 720;

    if (argc != 2) {
        std::cerr << "Incorrect usage: Must supply path to Tiled export Lua file." << std::endl;
        return -1;
    }

    try {

        const te::Initialization init;

        te::WindowPtr pWindow = te::createWindowOpenGL(
            "Map Runner",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN);

        std::shared_ptr<te::LuaGameState> pState(new te::LuaGameState(
            te::TMX(argv[1])));
        te::StateStack stateStack(pState);

        std::thread prompt([] {
            std::string line;
            do {
                if (line == "quit") {
                    std::cout << "See ya!" << std::endl;
                    break;
                }
                std::cout << "> ";
            } while (std::getline(std::cin, line));
        });

        te::executeStack(stateStack, *pWindow);
        prompt.join();

    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }
    return 0;
}
