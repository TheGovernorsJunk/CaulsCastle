#include "ecs.h"

#include "shader.h"
#include "texture_manager.h"
#include "mesh_manager.h"
#include "animation_factory.h"
#include "camera.h"
#include "transform_component.h"
#include "animation_component.h"
#include "data_component.h"
#include "command_component.h"
#include "entity_manager.h"
#include "command_system.h"
#include "render_system.h"

namespace te
{
    AssetManager::AssetManager(TMX&& tmx)
        : AssetManager(std::shared_ptr<const TMX>(new TMX(std::move(tmx))))
    {}

    AssetManager::AssetManager(std::shared_ptr<const TMX> pTMX)
        : pTextureManager(new TextureManager())
        , pMeshManager(new MeshManager(pTMX, pTextureManager))
        , pAnimationFactory(new AnimationFactory(pTMX, pMeshManager))
    {}

    ECS::ECS()
        : pTransformComponent(new TransformComponent())
        , pAnimationComponent(new AnimationComponent())
        , pDataComponent(new DataComponent())
        , pCommandComponent(new CommandComponent())
        , pEntityManager(new EntityManager(EntityManager::ObserverVector{
              pTransformComponent,
              pAnimationComponent,
              pDataComponent
          }))
    {}

    void processInput(const ECSWatchers& watchers, char ch, InputType type)
    {
        watchers.pInputSystem->processInput(ch, type);
    }

    void update(const ECSWatchers& watchers, float dt)
    {
        watchers.pCommandSystem->update(dt);
        watchers.pRenderSystem->update(dt);
    }

    void draw(const ECSWatchers& watchers, const glm::mat4& viewTransform)
    {
        watchers.pRenderSystem->draw(viewTransform * watchers.pCamera->getView());
    }

    ECSWatchers::ECSWatchers(ECS& ecs, std::shared_ptr<const Shader> pShader)
        : pCamera(new Camera())
        , pCommandSystem(new CommandSystem(ecs))
        , pInputSystem(new InputSystem(pCommandSystem))
        , pRenderSystem(new RenderSystem(ecs, pShader))
    {
        assert(pShader);
        ecs.pTransformComponent->addObserver(pCamera);
    }
}
