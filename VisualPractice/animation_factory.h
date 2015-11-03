#ifndef TE_ANIMATION_FACTORY
#define TE_ANIMATION_FACTORY

#include <memory>
#include <vector>
#include <map>

namespace te
{
    struct TMX;
    class Mesh;
    class MeshManager;

    struct Frame {
        std::shared_ptr<Mesh> mesh;
        unsigned duration;
    };

    class AnimationFactory
    {
    public:
        AnimationFactory(std::shared_ptr<const TMX> pTMX, std::shared_ptr<MeshManager> pMeshManager);

        std::vector<Frame> create(const std::map<std::string, std::string>& propertyMap) const;
    private:
        std::shared_ptr<const TMX> mpTMX;
        std::shared_ptr<MeshManager> mpMeshManager;
    };
}

#endif
