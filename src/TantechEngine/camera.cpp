#include "camera.h"

#include <glm/gtx/transform.hpp>

#include <iostream>

namespace te
{
    Camera::Camera()
        : mHasSubject(false)
        , mSubject()
    {}

    void Camera::follow(const Entity& entity)
    {
        mHasSubject = true;
        mSubject = entity;
    }

    void Camera::onNotify(const TransformUpdateEvent& evt)
    {
        if (mHasSubject && (evt.entity == mSubject)) {
            // TODO: Sophisticated implementation
            mViewTransform = glm::inverse(glm::translate(glm::vec3(evt.worldTransform[3])));
        }
    }

    glm::mat4 Camera::getView() const
    {
        return mViewTransform;
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
