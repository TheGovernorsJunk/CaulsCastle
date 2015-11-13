#ifndef TE_MESH_H
#define TE_MESH_H

#include "gl.h"
#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace te
{
    class Texture;

    struct Vertex {
        struct Position {
            GLfloat x;
            GLfloat y;
            GLfloat z;
        } position;
        struct TexCoords {
            GLfloat s;
            GLfloat t;
        } texCoords;
    };

    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices,
             const std::vector<GLuint>& indices,
             const std::vector<std::shared_ptr<const Texture>>& textures);
        ~Mesh();
        Mesh(Mesh&&);
        Mesh& operator=(Mesh&&);

        GLuint getVAO() const;
        GLuint getVBO() const;
        GLuint getEBO() const;
        GLsizei getElementCount() const;
        std::shared_ptr<const Texture> getTexture(unsigned i) const;

    private:
        void destroy();

        GLuint mVAO, mVBO, mEBO;
        GLsizei mElementCount;
        std::vector<std::shared_ptr<const Texture>> mTextures;

        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;
    };
}

#endif
