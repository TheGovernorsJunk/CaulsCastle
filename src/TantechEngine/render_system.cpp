#include "render_system.h"
#include "simple_render_component.h"
#include "transform_component.h"
#include "animation_component.h"
#include "shader.h"
#include "texture.h"
#include "model.h"

#include <glm/gtc/type_ptr.hpp>

namespace te
{
    RenderSystem::RenderSystem(
        std::shared_ptr<const Shader> pShader,
        std::shared_ptr<SimpleRenderComponent> pRender,
        std::shared_ptr<AnimationComponent> pAnimation,
        std::shared_ptr<TransformComponent> pTransform)
        : mpShader(pShader)
        , mpRender(pRender)
        , mpAnimation(pAnimation)
        , mpTransform(pTransform)
    {
        if (!mpTransform) {
            throw std::runtime_error{ "RenderSystem ctor: requires transform component." };
        }
    }

    void RenderSystem::update(float dt) const
    {
        if (mpAnimation) {
            mpAnimation->forEach([dt](const Entity& entity, AnimationInstance& instance) {
                // Frozen animations require no update
                if (instance.currAnimation->frozen) { return; }

                instance.currDuration += dt * 1000;
                if ((unsigned)instance.currDuration > instance.currAnimation->frames[instance.currFrameIndex].duration) {
                    instance.currDuration = 0;
                    ++instance.currFrameIndex;
                    if (instance.currFrameIndex >= instance.currAnimation->frames.size()) {
                        instance.currFrameIndex = 0;
                    }
                }
            });
        }
    }

    void RenderSystem::draw(const glm::mat4& viewTransform) const
    {
        if (mpRender) {
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
        }

        if (mpAnimation) {
            mpAnimation->forEach([&, this](const Entity& entity, AnimationInstance& instance) {
                instance.currAnimation->frames[instance.currFrameIndex].model->draw(*mpShader, viewTransform * mpTransform->getWorldTransform(entity));
            });
        }
    }
}
