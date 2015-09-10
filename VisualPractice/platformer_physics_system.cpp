#include "platformer_physics_system.h"
#include "physics_component.h"
#include "transform_component.h"
#include "bounding_box_component.h"
#include "tiled_map.h"
#include <glm/gtx/transform.hpp>

namespace te
{
    PlatformerPhysicsSystem::PlatformerPhysicsSystem(
        std::shared_ptr<PhysicsComponent> pPhysics,
        std::shared_ptr<TransformComponent> pTransform,
        std::shared_ptr<BoundingBoxComponent> pBoundingBox,
        std::shared_ptr<TiledMap> pTiledMap,
        float gravityAcceleration)
        : mpPhysics(pPhysics)
        , mpTransform(pTransform)
        , mpBoundingBox(pBoundingBox)
        , mpTiledMap(pTiledMap)
        , mGravityAcceleration(0, gravityAcceleration)
    {}

    void PlatformerPhysicsSystem::update(float dt) const
    {
        TransformComponent& transformComponent = *mpTransform;
        const BoundingBoxComponent& boundingBox = *mpBoundingBox;
        const glm::vec2& gravityAcceleration = mGravityAcceleration;
        const TiledMap& tiledMap = *mpTiledMap;

        mpPhysics->forEach([&transformComponent, &boundingBox, &gravityAcceleration, &tiledMap, dt](const Entity& entity, PhysicsInstance& instance)
        {
            instance.velocity += dt * gravityAcceleration;
            glm::vec3 directionVector(dt * glm::vec3(instance.velocity.x, instance.velocity.y, 0));
            transformComponent.setLocalTransform(
                entity,
                glm::translate(transformComponent.getLocalTransform(entity), directionVector));

            while (tiledMap.checkCollision(boundingBox.getBoundingBox(entity)))
            {
                instance.velocity.y = 0;
                BoundingBox intersection = tiledMap.getIntersections(boundingBox.getBoundingBox(entity))[0];
                float translateX = 0, translateY = 0;
                if (intersection.w < intersection.h)
                {
                    translateX = directionVector.x > 0 ?
                        -intersection.w :
                        intersection.w;
                }
                else
                {
                    translateY = directionVector.y > 0 ?
                        -intersection.h :
                        intersection.h;
                }
                transformComponent.setLocalTransform(
                    entity,
                    glm::translate(transformComponent.getLocalTransform(entity), glm::vec3(translateX, translateY, 0)));
            }
        });
    }
}
