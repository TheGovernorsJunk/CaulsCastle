#ifndef TE_COMMAND_COMPONENT_H
#define TE_COMMAND_COMPONENT_H

#include "typedefs.h"
#include "component.h"

#include <memory>

namespace te
{
    enum EntityType {
        HUMAN   = 0x01, // 00000001
        MONSTER = 0x02, // 00000010
    };

    struct CommandInstance {
        CommandMask commandMask;
    };

    class CommandComponent : public Component<CommandInstance> {
    public:
        CommandComponent(std::size_t capacity = 1024);

        void setTypeMask(const Entity& entity, CommandMask commandMask);
    };

    typedef std::shared_ptr<CommandComponent> CommandPtr;
}

#endif
