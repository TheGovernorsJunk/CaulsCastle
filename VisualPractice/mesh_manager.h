#ifndef TE_MESH_MANAGER_H
#define TE_MESH_MANAGER_H

#include <map>
#include <memory>

namespace te
{
    struct TMX;
    class TextureManager;
    class Mesh;

    class MeshManager {
    public:
        MeshManager(std::shared_ptr<const TMX>, std::shared_ptr<TextureManager>);

        std::shared_ptr<Mesh> operator[](unsigned);
    private:
        std::shared_ptr<const TMX> mpTMX;
        std::shared_ptr<TextureManager> mpTextureManager;
        std::map<unsigned, std::shared_ptr<Mesh>> mMeshes;

        MeshManager(const MeshManager&) = delete;
        MeshManager& operator=(const MeshManager&) = delete;
    };
}

#endif
