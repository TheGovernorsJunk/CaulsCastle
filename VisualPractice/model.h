#ifndef TE_MODEL_H
#define TE_MODEL_H

#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace te
{
    class Mesh;
    class Shader;

    class Model {
    public:
        Model(const std::vector<std::shared_ptr<const Mesh>>& meshes);
        Model(std::vector<std::shared_ptr<const Mesh>>&& meshes);

        void draw(Shader& shader, const glm::mat4& view) const;
    private:
        std::vector<std::shared_ptr<const Mesh>> mMeshes;
    };
}

#endif
