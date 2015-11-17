#ifndef TE_WRAPPERS_H
#define TE_WRAPPERS_H

#include <memory>
#include <string>
#include <SDL_render.h>
#include <SDL_ttf.h>
#include <ft2build.h>
#include FT_FREETYPE_H

struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_Color;

namespace te
{

    class Initialization
    {
    public:
        Initialization(FT_Library* ftLib = nullptr);
        ~Initialization();
    };

    typedef std::shared_ptr<SDL_Window> WindowPtr;
    WindowPtr wrapWindow(SDL_Window*);
    WindowPtr createWindowOpenGL(const char* title, int x, int y, int w, int h, Uint32 flags);

    typedef std::shared_ptr<SDL_Surface> SurfacePtr;
    SurfacePtr loadSurface(const std::string& path, const SDL_PixelFormat* format);

    typedef std::shared_ptr<SDL_Renderer> RendererPtr;
    RendererPtr createRenderer(WindowPtr, int index = 0, Uint32 flags = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

    typedef std::shared_ptr<SDL_Texture> TexturePtr;
    TexturePtr loadTexture(const std::string& path, RendererPtr pRenderer);

    typedef std::shared_ptr<TTF_Font> FontPtr;
    FontPtr loadFont(const std::string& path, int ptSize);

    SurfacePtr loadTextSurface(const std::string& text, FontPtr pFont, const SDL_Color& fontColor, int width);

    struct WindowContext
    {
        WindowPtr pWindow;
        SDL_GLContext context;
    };
}

#endif /* TE_WRAPPERS_H */
