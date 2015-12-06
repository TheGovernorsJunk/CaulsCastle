#include "camera.h"
#include "transform_component.h"

#include <glm/gtx/transform.hpp>

#include <iostream>

namespace te
{
    Camera::Camera(const ECS& ecs)
        : System(ecs)
        , mHasSubject(false)
        , mSubject()
    {}

    void Camera::follow(const Entity& entity)
    {
        mHasSubject = true;
        mSubject = entity;
    }

    glm::mat4 Camera::getView() const
    {
        if (mHasSubject) {
            // TODO: Sophisticated implementation
            return glm::inverse(glm::translate(glm::vec3(get<TransformComponent>().getWorldTransform(mSubject)[3])));
        } else {
            return glm::mat4();
        }
    }

    void* Camera::operator new(std::size_t sz)
    {
        return _aligned_malloc(sz, 16);
    }

    void Camera::operator delete(void* ptr)
    {
        _aligned_free(ptr);
    }
}
