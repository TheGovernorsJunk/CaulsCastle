#ifndef TE_RENDER_SYSTEM_H
#define TE_RENDER_SYSTEM_H

#include "system.h"

#include <glm/glm.hpp>
#include "gl.h"
#include <memory>

namespace te
{
    class Shader;
    class SimpleRenderComponent;
    class AnimationComponent;
    class TransformComponent;

    class RenderSystem : public System
    {
    public:
        RenderSystem(
            const ECS& ecs,
            std::shared_ptr<const Shader> pShader);

        void update(float dt) const;
        void draw(const glm::mat4& viewTransform = glm::mat4()) const;

    private:
        std::shared_ptr<const Shader> mpShader;
    };
}

#endif
