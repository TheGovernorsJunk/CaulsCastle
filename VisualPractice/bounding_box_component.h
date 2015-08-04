#ifndef TE_BOUNDING_BOX_COMPONENT_H
#define TE_BOUNDING_BOX_COMPONENT_H

#include <glm/glm.hpp>
#include <memory>
#include "component.h"

struct SDL_Rect;

namespace te
{
	class TransformComponent;

    struct BBInstance
    {
        glm::vec2 dimensions;
        glm::vec2 offset;
    };

    class BoundingBoxComponent : public Component<BBInstance>
    {
    public:
        BoundingBoxComponent(std::shared_ptr<TransformComponent> pTransform, std::size_t capacity = 1024);

        void setBoundingBox(const Entity& entity, const glm::vec2& dimensions, const glm::vec2& offset);
        SDL_Rect getBoundingBox(const Entity& entity) const;
	private:
		friend class CollisionSystem;

        std::shared_ptr<TransformComponent> mpTransform;
    };

    typedef std::shared_ptr<BoundingBoxComponent> BoundingBoxPtr;
}

#endif
