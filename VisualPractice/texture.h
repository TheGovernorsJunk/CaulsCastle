#ifndef TE_TEXTURE_H
#define TE_TEXTURE_H

#include <string>
#include "gl.h"

namespace te
{
    class Texture
    {
    public:
        Texture();
        Texture::Texture(GLuint* pixels, GLuint width, GLuint height);
        Texture(const std::string& path, GLuint format = GL_RGBA);
        Texture(const std::string& path, GLubyte r, GLubyte g, GLubyte b, GLubyte a = 0);

        Texture(Texture&&);
        Texture& operator=(Texture&&);

        ~Texture();

        GLuint getID() const;

        GLuint Texture::getImgWidth() const;
        GLuint Texture::getImgHeight() const;
        GLuint Texture::getTexWidth() const;
        GLuint Texture::getTexHeight() const;
    private:
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;

        GLuint mID;
        GLuint mImgWidth;
        GLuint mImgHeight;
        GLuint mTexWidth;
        GLuint mTexHeight;
    };

    GLuint powerOfTwo(GLuint n);
}

#endif /* TE_TEXTURE_H */
