#include "animation_factory.h"
#include "tmx.h"
#include "mesh_manager.h"
#include "model.h"

#include <algorithm>

namespace te
{
    AnimationFactory::AnimationFactory(std::shared_ptr<const TMX> pTMX, std::shared_ptr<MeshManager> pMeshManager)
        : mpTMX(pTMX), mpMeshManager(pMeshManager) {}

    Animation AnimationFactory::create(unsigned gid, bool frozen) const
    {
        const TMX::Tileset::Tile* pMatch = nullptr;
        const TMX::Tileset* pTileset = nullptr;

        std::for_each(std::begin(mpTMX->tilesets), std::end(mpTMX->tilesets), [&](const TMX::Tileset& tileset) {
            std::for_each(std::begin(tileset.tiles), std::end(tileset.tiles), [&](const TMX::Tileset::Tile& tile) {
                if (tile.id + tileset.firstgid == gid) {
                    pMatch = &tile;
                    pTileset = &tileset;
                }
            });
        });

        if (pMatch && pMatch->animation.size() > 0) {
            std::vector<Frame> frames;
            std::for_each(std::begin(pMatch->animation), std::end(pMatch->animation), [&, this](const TMX::Tileset::Tile::Frame& frame) {
                frames.push_back({ std::shared_ptr<const Model>(new Model{
                    std::vector<std::shared_ptr<const Mesh>>{
                        std::shared_ptr<const Mesh>{ (*mpMeshManager)[frame.tileid + pTileset->firstgid] }
                    }
                }), frame.duration });
            });
            return { frames, frozen };
        } else {
            return{ std::vector<Frame>{
                Frame{
                    std::shared_ptr<const Model>{
                        new Model{
                            std::vector<std::shared_ptr<const Mesh>>{
                                { (*mpMeshManager)[gid] }
                            }
                        }
                    }, 0
                }
            }, true };
        }
    }

    Animation AnimationFactory::create(const std::map<std::string, std::string>& propertyMap, bool frozen) const
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
                frames.push_back(Frame{
                    std::shared_ptr<const Model>{
                        new Model{
                            std::vector<std::shared_ptr<const Mesh>>{
                                { (*mpMeshManager)[frame.tileid + pTileset->firstgid] }
                            }
                        }
                    }, frame.duration
                });
            });
            if (frames.size() > 0) {
                return { frames, frozen };
            } else {
                // If no animation data, use tile itself and mark as frozen
                frames.push_back(Frame{
                    std::shared_ptr<const Model>{
                        new Model{
                            std::vector<std::shared_ptr<const Mesh>>{
                                {(*mpMeshManager)[pMatch->id + pTileset->firstgid]}
                            }
                        }
                    }, 0
                });
                return { frames, true };
            }
        } else {
            throw std::runtime_error{ "AnimationFactory::create: No tile matches given property map." };
        }
    }
}
