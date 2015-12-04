#include "model.h"
#include "shader.h"

#include <algorithm>

namespace te
{
    Model::Model(const std::vector<std::shared_ptr<const Mesh>>& meshes)
        : mMeshes(meshes) {}
    Model::Model(std::vector<std::shared_ptr<const Mesh>>&& meshes)
        : mMeshes(std::move(meshes)) {}

    void Model::draw(const Shader& shader, const glm::mat4& view) const
    {
        std::for_each(std::begin(mMeshes), std::end(mMeshes), [&](const std::shared_ptr<const Mesh>& pMesh) {
            shader.draw(view, *pMesh);
        });
    }
}
