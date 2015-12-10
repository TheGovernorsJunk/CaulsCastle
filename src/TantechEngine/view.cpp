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

    View::View()
        : View(FloatRect(0,0,0,0))
    {}

    View::View(const FloatRect& lens)
        : mLens(lens)
        , mViewport(0, 0, 1, 1)
    {}

    FloatRect View::getLens() const
    {
        return mLens;
    }

    void View::reset(const FloatRect& lens)
    {
        mLens = lens;
    }

    void View::setViewport(const FloatRect& viewport)
    {
        checkViewport(viewport);
        mViewport = viewport;
    }

    View::Lock View::activate(const View& view, const std::shared_ptr<Shader>& pShader, SDL_Window& window)
    {
        if (!pShader) { throw std::runtime_error("View::activate: Shader required."); }

        int width = 0, height = 0;
        SDL_GetWindowSize(&window, &width, &height);
        return activate(view, pShader, width, height);
    }

    View::Lock View::activate(const View& view, const std::shared_ptr<Shader>& pShader, int width, int height)
    {
        if (!pShader) { throw std::runtime_error("View::activate: Shader required."); }

        GLint originalViewport[4];
        glGetIntegerv(GL_VIEWPORT, originalViewport);

        FloatRect viewport = view.mViewport;
        glViewport((int)(viewport.x * width),
                   (int)(height - (height * (viewport.h + viewport.y))),
                   (int)(viewport.w * width),
                   (int)(viewport.h * height));

        glm::mat4 originalProjection = pShader->getProjection();
        pShader->setProjection(glm::ortho<GLfloat>(view.mLens.x, view.mLens.x + view.mLens.w, view.mLens.y + view.mLens.h, view.mLens.y, -Z, Z));

        return View::Lock(pShader, originalProjection, { originalViewport[0],
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
