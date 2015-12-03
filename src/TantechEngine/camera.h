#ifndef TE_CAMERA_H
#define TE_CAMERA_H

#include "transform_component.h"
#include "entity_manager.h"

#include <glm/glm.hpp>

namespace te
{
    class Camera : public Observer<TransformUpdateEvent>
    {
    public:
        Camera();

        void follow(const Entity&);
        void onNotify(const TransformUpdateEvent& evt);
        glm::mat4 getView() const;
    private:
        bool mHasSubject;
        Entity mSubject;
        glm::mat4 mViewTransform;
    };
}

#endif
