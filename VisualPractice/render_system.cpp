#include "render_system.h"
#include "simple_render_component.h"
#include "transform_component.h"
#include <glm/gtc/type_ptr.hpp>

namespace te
{
    RenderSystem::RenderSystem(
        std::shared_ptr<SimpleRenderComponent> pRender,
        std::shared_ptr<TransformComponent> pTransform)
        : mpRender(pRender)
        , mpTransform(pTransform)
    {}

    void RenderSystem::draw(const glm::mat4& viewTransform) const
    {
        glUseProgram(mpRender->mShader);

        glUniformMatrix4fv(mpRender->mViewLocation, 1, GL_FALSE, glm::value_ptr(viewTransform));

        GLuint modelLocation = mpRender->mModelLocation;
        TransformPtr pTransform = mpTransform;
        mpRender->forEach([modelLocation, pTransform](const Entity& entity, SimpleRenderInstance& instance)
        {
            glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(pTransform->getWorldTransform(entity)));

            glBindVertexArray(instance.vao);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            glBindVertexArray(0);
        });

        glUseProgram(0);
    }
}
