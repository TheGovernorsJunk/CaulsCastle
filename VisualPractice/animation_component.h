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
        std::vector<Frame> frames;
        float currDuration;
        unsigned currFrameIndex;
    };

    class AnimationComponent : public Component<AnimationInstance> {
    public:
        AnimationComponent(size_t capacity = 1024);

        void setAnimations(const Entity& entity, const std::vector<Frame>& frames);
        void setAnimations(const Entity& entity, std::vector<Frame>&& frames);
        //void setAnimations(
        //    const Entity& entity,
        //    const TMX& tmx,
        //    const TMX::Tileset::Tile::ObjectGroup::Object& object,
        //    MeshManager& meshManager);

    private:
        friend class RenderSystem;

        AnimationComponent(const AnimationComponent&) = delete;
        AnimationComponent& operator=(const AnimationComponent&) = delete;
    };

    typedef std::shared_ptr<AnimationComponent> AnimationPtr;
}

#endif
