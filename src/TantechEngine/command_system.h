#ifndef TE_COMMAND_SYSTEM_H
#define TE_COMMAND_SYSTEM_H

#include "typedefs.h"
#include "ecs.h"

#include <functional>
#include <memory>
#include <vector>

namespace te
{
    class Entity;
    class CommandComponent;

    class Command {
    public:
        Command(CommandMask dispatchMask, CommandMask forbidMask, std::function<void(const Entity&, const ECS&, float)> = [](const Entity&, const ECS&, float){});
        virtual ~Command();

    private:
        friend class CommandSystem;

        virtual void execute(const Entity& e, const ECS& ecs, float dt) const;

        std::function<void(const Entity&, const ECS&, float)> mFn;
        CommandMask mDispatchMask;
        CommandMask mForbidMask;
    };

    class CommandSystem
    {
    public:
        CommandSystem(const ECS&);

        void queueCommand(const Command&);
        void queueCommand(Command&&);
        void update(float dt);
    private:
        const ECS mECS;
        std::vector<Command> mCommands;
    };
}

#endif
