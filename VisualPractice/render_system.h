#ifndef TE_RENDER_SYSTEM_H
#define TE_RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include "gl.h"
#include <memory>

namespace te
{
    class SimpleRenderComponent;
    class TransformComponent;

    class RenderSystem
    {
    public:
        RenderSystem(
            std::shared_ptr<SimpleRenderComponent> pRender,
            std::shared_ptr<TransformComponent> pTransform);

        void draw(const glm::mat4& viewTransform) const;

    private:
        std::shared_ptr<SimpleRenderComponent> mpRender;
        std::shared_ptr<TransformComponent> mpTransform;
    };
}

#endif
