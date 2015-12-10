#ifndef TE_VIEW_H
#define TE_VIEW_H

#include "rect.h"

#include "gl.h"
#include <glm/glm.hpp>

#include <memory>

struct SDL_Window;

namespace te
{
    class Shader;

    class View
    {
    public:
        class Lock
        {
        public:
            ~Lock();
            Lock(Lock&&);
            Lock& operator=(Lock&&);
        private:
            friend class View;
            Lock(const std::shared_ptr<Shader>& pShader,
                 const glm::mat4& originalProjection,
                 const IntRect& originalViewport);

            std::shared_ptr<Shader> mpShader;
            glm::mat4 mProjection;
            IntRect mViewport;

            Lock(const Lock&) = delete;
            Lock& operator=(const Lock&) = delete;
        };

        View();
        View(const FloatRect& lens);

        FloatRect getLens() const;
        FloatRect getViewport() const;

        void reset(const FloatRect&);
        void setViewport(const FloatRect&);

        static Lock activate(const View&, const std::shared_ptr<Shader>& pShader, SDL_Window&);
        static Lock activate(const View&, const std::shared_ptr<Shader>& pShader, int screenWidth, int screenHeight);

    private:
        FloatRect mLens;
        FloatRect mViewport;
    };
}

#endif
