#include "wrappers.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
    }

    Initialization::~Initialization()
    {
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }


    WindowPtr wrapWindow(SDL_Window* w)
    {
        static auto deleter = [](SDL_Window* w) { SDL_DestroyWindow(w); };
        return WindowPtr(w, deleter);
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

}
