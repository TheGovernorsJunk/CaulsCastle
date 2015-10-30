#ifndef TE_ANIMATION_COMPONENT
#define TE_ANIMATION_COMPONENT

#include "component.h"
#include "mesh.h"
#include "tmx.h"

#include <vector>
#include <memory>

namespace te
{
    class MeshManager;

    struct AnimationInstance {
        struct Frame {
            std::shared_ptr<Mesh> mesh;
            unsigned duration;
        };
        std::vector<Frame> frames;
    };

    class AnimationComponent : public Component<AnimationInstance> {
    public:
        AnimationComponent(size_t capacity = 1024);

        void setAnimations(
            const Entity& entity,
            const TMX& tmx,
            const TMX::Tileset::Tile::ObjectGroup::Object& object,
            MeshManager& meshManager);
    };
}

#endif
