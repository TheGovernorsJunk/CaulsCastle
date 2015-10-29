#include "mesh.h"

namespace te
{
    Mesh::Mesh(const std::vector<Vertex>& vertices,
        const std::vector<GLuint>& indices,
        const std::vector<std::shared_ptr<const Texture>>& textures)
        : mVAO(0), mVBO(0), mEBO(0), mElementCount(indices.size()), mTextures(textures)
    {
        glGenVertexArrays(1, &mVAO);
        glGenBuffers(1, &mVBO);
        glGenBuffers(1, &mEBO);

        glBindVertexArray(mVAO);

        glBindBuffer(GL_ARRAY_BUFFER, mVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

        glBindVertexArray(0);
    }

    Mesh::~Mesh()
    {
        destroy();
    }

    Mesh::Mesh(Mesh&& o)
        : mVAO(o.mVAO)
        , mVBO(o.mVBO)
        , mEBO(o.mEBO)
        , mElementCount(o.mElementCount)
        , mTextures(std::move(o.mTextures))
    {
        o.mVAO = 0;
        o.mVBO = 0;
        o.mEBO = 0;
        o.mElementCount = 0;
    }

    Mesh& Mesh::operator=(Mesh&& o)
    {
        destroy();

        mVAO = o.mVAO;
        mVBO = o.mVBO;
        mEBO = o.mEBO;
        mElementCount = o.mElementCount;
        mTextures = std::move(o.mTextures);

        o.mVAO = 0;
        o.mVBO = 0;
        o.mEBO = 0;
        o.mElementCount = 0;

        return *this;
    }

    void Mesh::destroy()
    {
        glDeleteBuffers(1, &mVBO);
        glDeleteBuffers(1, &mEBO);
        glDeleteVertexArrays(1, &mVAO);
        mElementCount = 0;
        mTextures.clear();
    }

    GLuint Mesh::getVAO() const
    {
        return mVAO;
    }

    GLuint Mesh::getVBO() const
    {
        return mVBO;
    }

    GLuint Mesh::getEBO() const
    {
        return mEBO;
    }

    GLsizei Mesh::getElementCount() const
    {
        return mElementCount;
    }

    std::shared_ptr<const Texture> Mesh::getTexture(unsigned i) const
    {
        return mTextures.at(i);
    }
}
