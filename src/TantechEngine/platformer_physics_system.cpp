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

    void PlatformerPhysicsSystem::update(float dt, float padding)
    {
        mpPhysics->forEach([dt, padding, this](const Entity& entity, PhysicsInstance& instance)
        {
            instance.velocity += dt * mGravityAcceleration;
            glm::vec3 directionVector(dt * glm::vec3(instance.velocity.x, instance.velocity.y, 0));
            mpTransform->setLocalTransform(
                entity,
                glm::translate(mpTransform->getLocalTransform(entity), directionVector));

            while (mpTiledMap->getIntersections(mpBoundingBox->getBoundingBox(entity), mIntersections).size() > 0)
            {
                instance.velocity.y = 0;
                BoundingBox& intersection = mIntersections[0];
                float translateX = 0, translateY = 0;
                if (intersection.w < intersection.h)
                {
                    translateX = directionVector.x > 0 ?
                        -intersection.w - padding: // padding prevents floating point errors
                        intersection.w + padding;
                }
                else
                {
                    translateY = directionVector.y > 0 ?
                        -intersection.h - padding :
                        intersection.h + padding;
                }
                mpTransform->setLocalTransform(
                    entity,
                    glm::translate(mpTransform->getLocalTransform(entity), glm::vec3(translateX, translateY, 0)));
                mIntersections.clear();
            }
        });
    }
}
