#include "bounding_box_component.h"
#include "transform_component.h"
#include <array>

namespace te
{
    BoundingBox operator*(const glm::mat4& matrix, const BoundingBox& bb)
    {
        glm::vec4 topLeftVertex = matrix * glm::vec4(bb.x, bb.y, 0, 1);
        glm::vec4 topRightVertex = matrix * glm::vec4(bb.x + bb.w, bb.y, 0, 1);
        glm::vec4 bottomLeftVertex = matrix * glm::vec4(bb.x, bb.y + bb.h, 0, 1);
        return {
            topLeftVertex.x,
            topLeftVertex.y,
            topRightVertex.x - topLeftVertex.x,
            bottomLeftVertex.y - topLeftVertex.y
        };
    }

    BoundingBoxComponent::BoundingBoxComponent(
        std::shared_ptr<TransformComponent> pTransform,
        std::size_t capacity)
        : Component(capacity)
        , mpTransform(pTransform)
    {}

    void BoundingBoxComponent::setBoundingBox(const Entity& entity, const glm::vec2& dimensions, const glm::vec2& offset)
    {
        if (!hasInstance(entity))
        {
            createInstance(entity, { dimensions, offset });
        }
        else
        {
            at(entity) = { dimensions, offset };
        }
    }

	BoundingBox BoundingBoxComponent::getBoundingBox(const Entity& entity) const
	{
        if (!hasInstance(entity)) { throw std::out_of_range("No bounding box instance for entity."); }

        const BBInstance& instance = at(entity);

        glm::mat4 transform = mpTransform->getWorldTransform(entity);

        glm::vec4 vertices[4] = {
            transform * glm::vec4{ -(instance.dimensions.x) / 2 + instance.offset.x, -(instance.dimensions.y) / 2 + instance.offset.y, 0, 1 },
            transform * glm::vec4{  (instance.dimensions.x) / 2 + instance.offset.x, -(instance.dimensions.y) / 2 + instance.offset.y, 0, 1 },
            transform * glm::vec4{  (instance.dimensions.x) / 2 + instance.offset.x,  (instance.dimensions.y) / 2 + instance.offset.y, 0, 1 },
            transform * glm::vec4{ -(instance.dimensions.x) / 2 + instance.offset.x,  (instance.dimensions.y) / 2 + instance.offset.y, 0, 1 }
        };

        glm::vec2 min = { vertices[0].x, vertices[0].y };
        glm::vec2 max = { vertices[0].x, vertices[0].y };
        for (unsigned i = 0; i < 4; ++i)
        {
            if (vertices[i].x < min.x) { min.x = vertices[i].x; }
            if (vertices[i].y < min.y) { min.y = vertices[i].y; }
            if (vertices[i].x > max.x) { max.x = vertices[i].x; }
            if (vertices[i].y > max.y) { max.y = vertices[i].y; }
        }

        return {
            min.x,
            min.y,
            max.x - min.x,
            max.y - min.y
        };
	}

    //SDL_Rect BoundingBoxComponent::getBoundingBox(const Entity& entity) const
    //{
    //}
}
