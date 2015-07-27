#include "render_system.h"
#include "simple_render_component.h"
#include "transform_component.h"
#include <glm/gtc/type_ptr.hpp>

namespace te
{
    RenderSystem::RenderSystem(
        std::shared_ptr<SimpleRenderComponent> pRender,
        std::shared_ptr<TransformComponent> pTransform,
        const TransformationData& data)
        : mpRender(pRender)
        , mpTransform(pTransform)
        , mData(data)
    {}

    void RenderSystem::draw() const
    {
        TransformPtr pTransform = mpTransform;
        const TransformationData& data = mData;
        mpRender->forEach([&data, pTransform](const Entity& entity, SimpleRenderInstance& instance)
        {
            glm::mat4 modelViewMatrix = data.modelView * pTransform->getWorldTransform(entity);
            glUniformMatrix4fv(
                data.modelViewLocation,
                1,
                GL_FALSE,
                glm::value_ptr(modelViewMatrix));

            glEnableVertexAttribArray(data.vertex2DLocation);

            glBindBuffer(GL_ARRAY_BUFFER, instance.vbo);
            glVertexAttribPointer(data.vertex2DLocation, 2, GL_FLOAT, GL_FALSE, 0, NULL);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, instance.ibo);

            glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

            glDisableVertexAttribArray(data.vertex2DLocation);
        });
    }
}
