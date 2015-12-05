#include "command_system.h"
#include "command_component.h"

namespace te
{
    Command::Command(CommandMask dispatchMask, CommandMask forbidMask, std::function<void(const Entity&, const ECS&, float)> fn)
        : mDispatchMask(dispatchMask)
        , mForbidMask(forbidMask)
        , mFn(fn)
    {}

    Command::~Command() {}

    void Command::execute(const Entity& e, const ECS& ecs, float dt) const
    {
        mFn(e, ecs, dt);
    }

    CommandSystem::CommandSystem(const ECS& ecs)
        : System(ecs)
        , mECS(ecs)
        , mCommands()
    {}

    void CommandSystem::queueCommand(const Command& command)
    {
        mCommands.push_back(command);
    }

    void CommandSystem::queueCommand(Command&& command)
    {
        mCommands.push_back(std::move(command));
    }

    void CommandSystem::update(float dt)
    {
        get<CommandComponent>().forEach([this, dt](const te::Entity& e, CommandInstance inst) {
            std::for_each(std::begin(mCommands), std::end(mCommands), [&](const Command& command){
                if (((command.mDispatchMask & inst.commandMask) == command.mDispatchMask) &&
                    ((command.mForbidMask & inst.commandMask) == 0)) {
                    command.execute(e, mECS, dt);
                }
            });
        });
        mCommands.clear();
    }
}
