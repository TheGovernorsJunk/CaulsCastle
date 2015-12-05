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
        const ECS& ecs,
        std::shared_ptr<const Shader> pShader)
        : System(ecs)
        , mpShader(pShader)
    {}

    void RenderSystem::update(float dt) const
    {
        get<AnimationComponent>().forEach([dt](const Entity& entity, AnimationInstance& instance) {
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

    void RenderSystem::draw(const glm::mat4& viewTransform) const
    {
        get<AnimationComponent>().forEach([&, this](const Entity& entity, AnimationInstance& instance) {
            instance.currAnimation->frames[instance.currFrameIndex].model->draw(*mpShader, viewTransform * get<TransformComponent>().getWorldTransform(entity));
        });
    }
}
