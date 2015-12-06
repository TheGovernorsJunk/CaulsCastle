#ifndef TE_CAMERA_H
#define TE_CAMERA_H

#include "entity_manager.h"
#include "system.h"

#include <glm/glm.hpp>

namespace te
{
    class Camera : public System
    {
    public:
        Camera(const ECS&);

        void follow(const Entity&);
        glm::mat4 getView() const;

        static void* operator new(std::size_t);
        static void operator delete(void*);
    private:
        bool mHasSubject;
        Entity mSubject;
    };
}

#endif
