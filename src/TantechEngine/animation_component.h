#ifndef TE_ANIMATION_COMPONENT
#define TE_ANIMATION_COMPONENT

#include "component.h"
#include "mesh.h"
#include "tmx.h"
#include "animation_factory.h"

#include <vector>
#include <memory>

namespace te
{
    struct AnimationInstance {
        std::map<int, std::shared_ptr<const Animation>> animations;
        const Animation* currAnimation;
        float currDuration;
        unsigned currFrameIndex;
    };

    class AnimationComponent : public Component<AnimationInstance> {
    public:
        AnimationComponent(size_t capacity = 1024);

        void setAnimations(const Entity& entity, const std::map<int, std::shared_ptr<const Animation>>& animations, int initialKey);
        void setAnimations(const Entity& entity, std::map<int, std::shared_ptr<const Animation>>&& animations, int initialKey);
        void setAnimation(const Entity& entity, int key);

    private:
        friend class RenderSystem;

        AnimationComponent(const AnimationComponent&) = delete;
        AnimationComponent& operator=(const AnimationComponent&) = delete;
    };

    typedef std::shared_ptr<AnimationComponent> AnimationPtr;
}

#endif
