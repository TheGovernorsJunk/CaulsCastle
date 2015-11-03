#include "animation_component.h"
#include "mesh_manager.h"

namespace te
{
    AnimationComponent::AnimationComponent(size_t capacity)
        : Component(capacity) {}

    void AnimationComponent::setAnimations(const Entity& entity, const std::vector<Frame>& frames)
    {
        if (frames.size() == 0) { throw std::runtime_error{ "AnimationComponent::setAnimations: must have at least one frame." }; }
        if (!hasInstance(entity)) {
            createInstance(entity, { frames, 0, 0 });
        } else {
            at(entity) = { frames, 0, 0 };
        }
    }

    void AnimationComponent::setAnimations(const Entity& entity, std::vector<Frame>&& frames)
    {
        if (frames.size() == 0) { throw std::runtime_error{ "AnimationComponent::setAnimations: must have at least one frame." }; }
        if (!hasInstance(entity)) {
            createInstance(entity, { std::move(frames), 0, 0 });
        } else {
            at(entity) = { std::move(frames), 0, 0 };
        }
    }

    //void AnimationComponent::setAnimations(
    //    const Entity& entity,
    //    const TMX& tmx,
    //    const TMX::Tileset::Tile::ObjectGroup::Object& object,
    //    MeshManager& meshManager)
    //{
    //    AnimationInstance instance;

    //    unsigned tilesetIndex = getTilesetIndex(tmx, object.gid);
    //    const TMX::Tileset& tileset = tmx.tilesets.at(tilesetIndex);

    //    const TMX::Tileset::Tile* pObjectTile = nullptr;
    //    std::for_each(
    //        std::begin(tileset.tiles),
    //        std::end(tileset.tiles),
    //        [&pObjectTile, &object, &tileset](const TMX::Tileset::Tile& tile) {

    //        if (tile.id == object.gid - tileset.firstgid && tile.animation.size() > 0) {
    //            pObjectTile = &tile;
    //        }
    //    });

    //    if (pObjectTile) {
    //        std::for_each(
    //            std::begin(pObjectTile->animation),
    //            std::end(pObjectTile->animation),
    //            [&](const TMX::Tileset::Tile::Frame& frame) {

    //            instance.frames.push_back(AnimationInstance::Frame{
    //                meshManager[frame.tileid + tileset.firstgid],
    //                frame.duration
    //            });
    //        });
    //    } else {
    //        instance.frames.push_back(AnimationInstance::Frame{
    //            meshManager[object.gid],
    //            0
    //        });
    //    }

    //    if (!hasInstance(entity)) {
    //        createInstance(entity, std::move(instance));
    //    } else {
    //        at(entity) = std::move(instance);
    //    }
    //}
}
