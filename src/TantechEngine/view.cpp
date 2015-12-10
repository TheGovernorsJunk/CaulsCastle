#include "view.h"
#include "shader.h"

#include <SDL.h>
#include "gl.h"
#include <glm/gtx/transform.hpp>

#include <stdexcept>

namespace te
{
    const static GLfloat Z = 100;

    static void checkViewport(const FloatRect& viewport)
    {
        if (viewport.x < 0 ||
            viewport.y < 0 ||
            viewport.w > 1 ||
            viewport.y > 1)
        {
            throw std::runtime_error("View: Viewport must be in range [0,1].");
        }
    }

    View::View(const std::shared_ptr<Shader>& pShader)
        : View(pShader, FloatRect(0,0,0,0))
    {}

    View::View(const std::shared_ptr<Shader>& pShader, const FloatRect& lens)
        : mpShader(pShader)
        , mLens(lens)
        , mViewport(0, 0, 1, 1)
    {}

    void View::reset(const FloatRect& lens)
    {
        mLens = lens;
    }

    void View::setViewport(const FloatRect& viewport)
    {
        checkViewport(viewport);
        mViewport = viewport;
    }

    View::Lock View::activate(const View& view, SDL_Window& window)
    {
        int width = 0, height = 0;
        SDL_GetWindowSize(&window, &width, &height);
        return activate(view, width, height);
    }

    View::Lock View::activate(const View& view, int width, int height)
    {
        GLint originalViewport[4];
        glGetIntegerv(GL_VIEWPORT, originalViewport);

        FloatRect viewport = view.mViewport;
        glViewport((int)(viewport.x * width),
                   (int)(height - (height * (viewport.h + viewport.y))),
                   (int)(viewport.w * width),
                   (int)(viewport.h * height));

        glm::mat4 originalProjection = view.mpShader->getProjection();
        view.mpShader->setProjection(glm::ortho<GLfloat>(view.mLens.x, view.mLens.x + view.mLens.w, view.mLens.y + view.mLens.h, view.mLens.y, -Z, Z));

        return View::Lock(view.mpShader, originalProjection, { originalViewport[0],
                                                               originalViewport[1],
                                                               originalViewport[2],
                                                               originalViewport[3] });
    }

    View::Lock::Lock(const std::shared_ptr<Shader>& pShader,
                     const glm::mat4& originalProjection,
                     const IntRect& originalViewport)
        : mpShader(pShader)
        , mProjection(originalProjection)
        , mViewport(originalViewport)
    {
        assert(pShader);
    }

    View::Lock& View::Lock::operator=(Lock&& o)
    {
        mpShader = o.mpShader;
        mProjection = o.mProjection;
        mViewport = o.mViewport;

        o.mpShader = nullptr;

        return *this;
    }

    View::Lock::Lock(Lock&& o)
        : mpShader(o.mpShader)
        , mProjection(o.mProjection)
        , mViewport(o.mViewport)
    {
        o.mpShader = nullptr;
    }

    View::Lock::~Lock()
    {
        // Null mpShader indicates moved lock
        if (mpShader) {
            // Restore lens and viewport 
            mpShader->setProjection(mProjection);
            glViewport(mViewport.x, mViewport.y, mViewport.w, mViewport.h);
        }
    }
}
