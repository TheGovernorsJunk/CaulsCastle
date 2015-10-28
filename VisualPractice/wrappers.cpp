#include "wrappers.h"
#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <SDL_image.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <bass.h>
#include <iostream>
#include <exception>

namespace te
{

    Initialization::Initialization()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            throw std::runtime_error(SDL_GetError());
        }
        int imgFlags = IMG_INIT_PNG;
        if (IMG_Init(IMG_INIT_JPG) & imgFlags)
        {
            throw std::runtime_error(IMG_GetError());
        }
        if (TTF_Init() == -1)
        {
            throw std::runtime_error(TTF_GetError());
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        ilInit();
        iluInit();
        ilClearColour(255, 255, 255, 000);

        ILenum ilError = ilGetError();
        if (ilError != IL_NO_ERROR)
        {
            throw std::runtime_error("Unable to initialize DevIL.");
        }

        BASS_Init(-1, 44100, 0, NULL, NULL);
    }

    Initialization::~Initialization()
    {
        BASS_Free();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }


    WindowPtr wrapWindow(SDL_Window* w)
    {
        static auto deleter = [](SDL_Window* w) { SDL_DestroyWindow(w); };
        return WindowPtr(w, deleter);
    }

    WindowPtr createWindowOpenGL(const char* title, int x, int y, int w, int h, Uint32 flags)
    {
        WindowPtr pW(
            SDL_CreateWindow(title, x, y, w, h, flags | SDL_WINDOW_OPENGL),
            [](SDL_Window* w){ SDL_DestroyWindow(w); });

        SDL_GLContext context(SDL_GL_CreateContext(pW.get()));

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        glewExperimental = GL_TRUE;
        GLenum glewError = glewInit();
        if (glewError != GLEW_OK)
        {
            throw std::runtime_error(std::string((const char*)glewGetErrorString(glewError)));
        }

        return pW;
    }

    auto surfaceDeleter = [](SDL_Surface* s) { SDL_FreeSurface(s); };

    SurfacePtr loadSurface(const std::string& path, const SDL_PixelFormat* format)
    {
        SurfacePtr pLoadedSurface(IMG_Load(path.c_str()), surfaceDeleter);

        if (pLoadedSurface.get() == NULL)
        {
            throw std::runtime_error(IMG_GetError());
        }

        SurfacePtr pOptimizedSurface(
            SDL_ConvertSurface(
                pLoadedSurface.get(),
                format,
                NULL
            ),
            surfaceDeleter
        );

        if (pOptimizedSurface.get() == NULL)
        {
            throw std::runtime_error(SDL_GetError());
        }

        return pOptimizedSurface;
    }

    RendererPtr createRenderer(WindowPtr pWindow, int index, Uint32 flags)
    {
        auto deleter = [](SDL_Renderer* r) { SDL_DestroyRenderer(r); };
        return RendererPtr(
            SDL_CreateRenderer(pWindow.get(), index, flags),
            deleter
        );
    }

    TexturePtr loadTexture(const std::string& path, RendererPtr pRenderer)
    {
        static auto textureDeleter = [](SDL_Texture* t) { SDL_DestroyTexture(t); };

        SurfacePtr pLoadedSurface(IMG_Load(path.c_str()), surfaceDeleter);
        if (pLoadedSurface.get() == NULL)
        {
            throw std::runtime_error(IMG_GetError());
        }

        TexturePtr pTexture(
            SDL_CreateTextureFromSurface(pRenderer.get(), pLoadedSurface.get()),
            textureDeleter
        );
        if (pTexture.get() == NULL)
        {
            throw std::runtime_error(SDL_GetError());
        }

        return pTexture;
    }

    FontPtr loadFont(const std::string& path, int ptSize)
    {
        return FontPtr(TTF_OpenFont(path.c_str(), ptSize), &TTF_CloseFont);
    }

    SurfacePtr loadTextSurface(const std::string& text, FontPtr pFont, const SDL_Color& fontColor, int width)
    {
        SurfacePtr pTextSurface(
            TTF_RenderText_Blended_Wrapped(pFont.get(), text.c_str(), fontColor, width),
            &SDL_FreeSurface);

        if (pTextSurface.get() == NULL)
        {
            throw std::runtime_error(TTF_GetError());
        }

        return pTextSurface;
    }

}
