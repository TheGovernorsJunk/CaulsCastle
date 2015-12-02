#ifndef TE_ECS_H
#define TE_ECS_H

#include <memory>

namespace te
{
    struct TMX;

    class Shader;

    class TextureManager;
    class MeshManager;
    class AnimationFactory;

    struct AssetManager {
        const std::shared_ptr<TextureManager> pTextureManager;
        const std::shared_ptr<MeshManager> pMeshManager;
        const std::shared_ptr<AnimationFactory> pAnimationFactory;

        AssetManager(TMX&&);
        AssetManager(std::shared_ptr<const TMX>);
    };

    class TransformComponent;
    class AnimationComponent;
    class DataComponent;

    class EntityManager;

    class RenderSystem;

    struct ECS {
        AssetManager assets;

        const std::shared_ptr<TransformComponent> pTransformComponent;
        const std::shared_ptr<AnimationComponent> pAnimationComponent;
        const std::shared_ptr<DataComponent> pDataComponent;

        const std::shared_ptr<EntityManager> pEntityManager;

        const std::shared_ptr<RenderSystem> pRenderSystem;

        ECS(std::shared_ptr<TMX>, std::shared_ptr<Shader>);
        ECS(std::shared_ptr<Shader>,
            const AssetManager&);
    };
}

#endif
