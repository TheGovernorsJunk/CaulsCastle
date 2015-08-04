#ifndef TE_RENDER_SYSTEM_H
#define TE_RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include "gl.h"
#include <memory>

namespace te
{
    class SimpleRenderComponent;
    class TransformComponent;

    struct TransformationData
    {
        glm::mat4 projection;
        glm::mat4 modelView;

        GLint projectionLocation;
        GLint modelViewLocation;

        GLint vertex2DLocation;
    };

    class RenderSystem
    {
    public:
        RenderSystem(
            std::shared_ptr<SimpleRenderComponent> pRender,
            std::shared_ptr<TransformComponent> pTransform,
            const TransformationData& data);

        void draw() const;

    private:
        std::shared_ptr<SimpleRenderComponent> mpRender;
        std::shared_ptr<TransformComponent> mpTransform;
        TransformationData mData;
    };
}

#endif
