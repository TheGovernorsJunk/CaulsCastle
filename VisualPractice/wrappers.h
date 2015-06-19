#ifndef TE_WRAPPERS_H
#define TE_WRAPPERS_H

#include <memory>
#include <string>
#include <SDL_render.h>

struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Texture;

namespace te
{

    class Initialization
    {
    public:
        Initialization();
        ~Initialization();
    };

    typedef std::shared_ptr<SDL_Window> WindowPtr;
    WindowPtr wrapWindow(SDL_Window*);

    typedef std::shared_ptr<SDL_Surface> SurfacePtr;
    SurfacePtr loadSurface(const std::string& path, const SDL_PixelFormat* format);

    typedef std::shared_ptr<SDL_Renderer> RendererPtr;
    RendererPtr createRenderer(WindowPtr, int index = 0, Uint32 flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    typedef std::shared_ptr<SDL_Texture> TexturePtr;
    TexturePtr loadTexture(const std::string& path, RendererPtr pRenderer);
}

#endif /* TE_WRAPPERS_H */
