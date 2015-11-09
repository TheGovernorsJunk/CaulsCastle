#include "texture.h"
#include <stdexcept>
#include <IL/il.h>
#include <IL/ilu.h>
#include <vector>
#include <algorithm>
#include <iostream>

namespace te
{
    GLuint powerOfTwo(GLuint num)
    {
        if (num != 0)
        {
            --num;
            num |= (num >> 1);
            num |= (num >> 2);
            num |= (num >> 4);
            num |= (num >> 8);
            num |= (num >> 16);
            ++num;
        }
        return num;
    }

    Texture::Texture()
        : mID(0), mImgWidth(0), mImgHeight(0), mTexWidth(0), mTexHeight(0) {}

    Texture::Texture(Texture&& o)
        : mID(o.mID), mImgWidth(o.mImgWidth), mImgHeight(o.mImgHeight), mTexWidth(o.mTexWidth), mTexHeight(o.mTexHeight)
    {
        o.mID = 0;
        o.mImgWidth = 0;
        o.mImgHeight = 0;
        o.mTexWidth = 0;
        o.mTexHeight = 0;
    }

    Texture& Texture::operator=(Texture&& o)
    {
        glDeleteTextures(1, &mID);

        mID = o.mID;
        mImgWidth = o.mImgWidth;
        mImgHeight = o.mImgHeight;
        mTexWidth = o.mTexWidth;
        mTexHeight = o.mTexHeight;

        o.mID = 0;
        o.mImgWidth = 0;
        o.mImgHeight = 0;
        o.mTexWidth = 0;
        o.mTexHeight = 0;

        return *this;
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &mID);
    }

    GLuint loadTexture32(GLuint* pixels, GLuint width, GLuint height)
    {
        GLuint texID;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, NULL);

        //GLenum error = glGetError();
        //if (error != GL_NO_ERROR) throw std::runtime_error("Error loading texture from pixels");

        return texID;
    }

    std::vector<GLuint> loadPixels32(const std::string& path, GLuint& imgWidth, GLuint& imgHeight, GLuint& texWidth, GLuint& texHeight)
    {
        ILuint imgID = 0;
        ilGenImages(1, &imgID);
        ilBindImage(imgID);

        ILboolean success = ilLoadImage(path.c_str());
        if (success != IL_TRUE) throw std::runtime_error("Error loading image");

        success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
        if (success != IL_TRUE) throw std::runtime_error("Error converting image");

        imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
        imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

        texWidth = powerOfTwo(imgWidth);
        texHeight = powerOfTwo(imgHeight);

        if (imgWidth != texWidth || imgHeight != texHeight)
        {
            iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
            iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
        }

        GLuint size = texWidth * texHeight;
        std::vector<GLuint> pixels(size);

        std::memcpy(pixels.data(), ilGetData(), size * 4);

        ilDeleteImages(1, &imgID);

        return pixels;
    }

    GLuint loadTexture8(GLubyte* pixels, GLuint width, GLuint height)
    {
        GLuint texID = 0;
        glGenTextures(1, &texID);
        glBindTexture(GL_TEXTURE_2D, texID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindTexture(GL_TEXTURE_2D, NULL);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) throw std::runtime_error("Unable to load texture.");

        return texID;
    }

    std::vector<GLubyte> loadPixels8(const std::string& path, GLuint& imgWidth, GLuint& imgHeight, GLuint& texWidth, GLuint& texHeight)
    {
        ILuint imgID = 0;
        ilGenImages(1, &imgID);
        ilBindImage(imgID);

        ILboolean success = ilLoadImage(path.c_str());
        if (success != IL_TRUE) throw std::runtime_error("Unable to load image.");

        success = ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
        if (success != IL_TRUE) throw std::runtime_error("Unable to load image.");

        imgWidth = (GLuint)ilGetInteger(IL_IMAGE_WIDTH);
        imgHeight = (GLuint)ilGetInteger(IL_IMAGE_HEIGHT);

        texWidth = powerOfTwo(imgWidth);
        texHeight = powerOfTwo(imgHeight);

        if (imgWidth != texWidth || imgHeight != texHeight)
        {
            iluImageParameter(ILU_PLACEMENT, ILU_UPPER_LEFT);
            iluEnlargeCanvas((int)texWidth, (int)texHeight, 1);
        }

        GLuint size = texWidth * texHeight;
        std::vector<GLubyte> pixels(size, 0);

        memcpy(pixels.data(), ilGetData(), size);

        ilDeleteImages(1, &imgID);

        return pixels;
    }

    Texture::Texture(GLuint* pixels, GLuint width, GLuint height)
        : mID(loadTexture32(pixels, width, height))
        , mImgWidth(width)
        , mImgHeight(height)
        , mTexWidth(width)
        , mTexHeight(height)
    {}

    Texture::Texture(const std::string& path, GLuint format)
        : mID(0), mImgWidth(0), mImgHeight(0), mTexWidth(0), mTexHeight(0)
    {
        if (format == GL_RGBA) {
            std::vector<GLuint> pixels(loadPixels32(path, mImgWidth, mImgHeight, mTexWidth, mTexHeight));
            mID = loadTexture32(pixels.data(), mTexWidth, mTexHeight);
        } else if (format == GL_ALPHA) {
            std::vector<GLubyte> pixels{ loadPixels8(path, mImgWidth, mImgHeight, mTexWidth, mTexHeight) };
            mID = loadTexture8(pixels.data(), mTexWidth, mTexHeight);
        }
    }

    void Texture::loadWithColorMask(const std::string& path, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
    {
        std::vector<GLuint> pixels(loadPixels32(path, mImgWidth, mImgHeight, mTexWidth, mTexHeight));

        std::for_each(std::begin(pixels), std::end(pixels), [r,g,b,a](GLuint& pixel)
        {
            GLubyte* colors = (GLubyte*)&pixel;
            if (colors[0] == r && colors[1] == g && colors[2] == b && (a == 0 || colors[3] == a))
            {
                colors[0] = 255;
                colors[1] = 255;
                colors[2] = 255;
                colors[3] = 000;
            }
        });

        mID = loadTexture32(pixels.data(), mTexWidth, mTexHeight);
    }

    Texture::Texture(const std::string& path, GLubyte r, GLubyte g, GLubyte b, GLubyte a)
        : mID(0), mImgWidth(0), mImgHeight(0), mTexWidth(0), mTexHeight(0)
    {
        loadWithColorMask(path, r, g, b, a);
    }

    Texture::Texture(const TMX::Tileset& tileset)
        : mID(0), mImgWidth(0), mImgHeight(0), mTexWidth(0), mTexHeight(0)
    {
        if (tileset.transparentcolor.a) {
            loadWithColorMask(
                tileset.image,
                tileset.transparentcolor.r,
                tileset.transparentcolor.g,
                tileset.transparentcolor.b);
        } else {
            std::vector<GLuint> pixels(loadPixels32(tileset.image, mImgWidth, mImgHeight, mTexWidth, mTexHeight));
            mID = loadTexture32(pixels.data(), mTexWidth, mTexHeight);
        }
    }

    GLuint Texture::getID() const
    {
        return mID;
    }

    GLuint Texture::getImgWidth() const
    {
        return mImgWidth;
    }

    GLuint Texture::getImgHeight() const
    {
        return mImgHeight;
    }

    GLuint Texture::getTexWidth() const
    {
        return mTexWidth;
    }

    GLuint Texture::getTexHeight() const
    {
        return mTexHeight;
    }
}
