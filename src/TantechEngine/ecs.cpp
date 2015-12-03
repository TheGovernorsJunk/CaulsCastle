#include "ecs.h"

#include "shader.h"
#include "texture_manager.h"
#include "mesh_manager.h"
#include "animation_factory.h"
#include "transform_component.h"
#include "animation_component.h"
#include "data_component.h"
#include "entity_manager.h"
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

    ECS::ECS(std::shared_ptr<Shader> pShader)
        : pTransformComponent(new TransformComponent())
        , pAnimationComponent(new AnimationComponent())
        , pDataComponent(new DataComponent())
        , pEntityManager(new EntityManager(EntityManager::ObserverVector{
              pTransformComponent,
              pAnimationComponent,
              pDataComponent
          }))
        , pRenderSystem(new RenderSystem(pShader, nullptr, pAnimationComponent, pTransformComponent))
    {}

    void update(const ECS& ecs, float dt)
    {
        ecs.pRenderSystem->update(dt);
    }

    void draw(const ECS& ecs, const glm::mat4& viewTransform)
    {
        ecs.pRenderSystem->draw(viewTransform);
    }
}
