#ifndef TE_ECS_H
#define TE_ECS_H

#include <glm/glm.hpp>

#include <memory>
#include <string>

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

    class Camera;

    class TransformComponent;
    class AnimationComponent;
    class DataComponent;

    class EntityManager;

    class RenderSystem;

    struct ECS {
        ECS();

        const std::shared_ptr<TransformComponent> pTransformComponent;
        const std::shared_ptr<AnimationComponent> pAnimationComponent;
        const std::shared_ptr<DataComponent> pDataComponent;

        const std::shared_ptr<EntityManager> pEntityManager;
    };

    struct ECSWatchers {
        ECSWatchers(ECS& ecs, std::shared_ptr<const Shader>);

        const std::shared_ptr<Camera> pCamera;
        const std::shared_ptr<RenderSystem> pRenderSystem;
    };

    void update(const ECSWatchers&, float dt);
    void draw(const ECSWatchers&, const glm::mat4& viewTransform = glm::mat4());

    class LuaStateECS {
    public:
        LuaStateECS(const ECS&, const ECSWatchers&);
        ~LuaStateECS();

        void loadScript(const std::string& path) const;
        void runScript() const;
        void runConsole() const;
    private:
        // May add lots of scripting, so avoid recompilation
        // with private implementation.
        struct Impl;
        // Use of unique_ptr causes awkward compilation errors
        Impl* mpImpl;
    };
}

#endif
