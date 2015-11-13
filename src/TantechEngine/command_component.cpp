#include "command_component.h"

namespace te
{
    CommandComponent::CommandComponent(std::size_t capacity)
        : Component(capacity) {}

    void CommandComponent::setTypeMask(const Entity& entity, CommandMask typeMask)
    {
        if (!hasInstance(entity)) {
            createInstance(entity, { typeMask });
        } else {
            at(entity) = { typeMask };
        }
    }

}
