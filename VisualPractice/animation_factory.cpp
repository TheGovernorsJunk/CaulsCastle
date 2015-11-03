#include "animation_factory.h"
#include "tmx.h"
#include "mesh_manager.h"

#include <algorithm>

namespace te
{
    AnimationFactory::AnimationFactory(std::shared_ptr<const TMX> pTMX, std::shared_ptr<MeshManager> pMeshManager)
        : mpTMX(pTMX), mpMeshManager(pMeshManager) {}

    std::vector<Frame> AnimationFactory::create(const std::map<std::string, std::string>& propertyMap) const
    {
        const TMX::Tileset::Tile* pMatch = nullptr;
        const TMX::Tileset* pTileset = nullptr;

        std::for_each(std::begin(mpTMX->tilesets), std::end(mpTMX->tilesets), [&](const TMX::Tileset& tileset) {
            std::for_each(std::begin(tileset.tiles), std::end(tileset.tiles), [&](const TMX::Tileset::Tile& tile) {

                for (auto it = propertyMap.begin(); it != propertyMap.end(); ++it) {
                    auto matchCandidate = tile.properties.find(it->first);
                    if (!(matchCandidate != tile.properties.end() &&
                          matchCandidate->second == it->second)) {
                        return; // exit function: this tile does not qualify
                    }
                }

                if (!pMatch) {
                    // Match: function would have exited if not
                    pMatch = &tile;
                    pTileset = &tileset;
                } else {
                    throw std::runtime_error{ "AnimationFactory::create: More than one tile matches given property map." };
                }
            });
        });

        if (pMatch) {
            std::vector<Frame> frames;
            std::for_each(std::begin(pMatch->animation), std::end(pMatch->animation), [&, this](const TMX::Tileset::Tile::Frame& frame) {
                frames.push_back({(*mpMeshManager)[frame.tileid + pTileset->firstgid], frame.duration});
            });
            if (frames.size() > 0) {
                return frames;
            } else {
                throw std::runtime_error{ "AnimationFactory::create: No animation for matching tile." };
            }
        } else {
            throw std::runtime_error{ "AnimationFactory::create: No tile matches given property map." };
        }
    }
}
