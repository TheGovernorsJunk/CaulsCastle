#include "commands.h"
#include "ecs.h"
#include "transform_component.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <iostream>

namespace te
{
    static std::function<void(const Entity&, const ECS&, float)> genMove(luabridge::LuaRef args)
    {
        if (!args.isTable()) {
            throw std::runtime_error("Function requires array of arguments.");
        }
        glm::vec3 vel = args[1];
        return [vel](const Entity& entity, const ECS& ecs, float) {
            ecs.pTransformComponent->multiplyTransform(entity, glm::translate(vel), TransformComponent::Space::WORLD);
        };
    }

    std::function<void(const Entity&, const ECS&, float)>(*getFunction(FuncID id)) (luabridge::LuaRef)
    {
        switch (id) {
        case FuncID::MOVE:
            return &genMove;
        default:
            throw std::runtime_error("No function for given ID");
        }
    }
}
