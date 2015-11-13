#ifndef TE_COMMAND_SYSTEM_H
#define TE_COMMAND_SYSTEM_H

#include "typedefs.h"

#include <functional>
#include <memory>
#include <vector>

namespace te
{
    class Entity;
    class CommandComponent;

    class Command {
    public:
        Command(CommandMask dispatchMask, CommandMask forbidMask, std::function<void(const Entity&, float)> = [](const Entity&, float){});
        virtual ~Command();

    private:
        friend class CommandSystem;

        virtual void execute(const Entity& e, float dt) const;

        std::function<void(const Entity&, float)> mFn;
        CommandMask mDispatchMask;
        CommandMask mForbidMask;
    };

    class CommandSystem
    {
    public:
        CommandSystem(std::shared_ptr<CommandComponent>);

        void queueCommand(const Command&);
        void queueCommand(Command&&);
        void update(float dt);
    private:
        std::shared_ptr<CommandComponent> mpCommandComponent;
        std::vector<Command> mCommands;
    };
}

#endif
