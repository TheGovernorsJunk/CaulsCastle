#ifndef TE_RENDER_SYSTEM_H
#define TE_RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include "gl.h"
#include <memory>

namespace te
{
    class Shader;
    class SimpleRenderComponent;
    class AnimationComponent;
    class TransformComponent;

    class RenderSystem
    {
    public:
        RenderSystem(
            std::shared_ptr<Shader> pShader,
            std::shared_ptr<SimpleRenderComponent> pRender,
            std::shared_ptr<AnimationComponent> pAnimation,
            std::shared_ptr<TransformComponent> pTransform);

        void update(float dt) const;
        void draw(const glm::mat4& viewTransform) const;

    private:
        std::shared_ptr<Shader> mpShader;
        std::shared_ptr<SimpleRenderComponent> mpRender;
        std::shared_ptr<AnimationComponent> mpAnimation;
        std::shared_ptr<TransformComponent> mpTransform;
    };
}

#endif
