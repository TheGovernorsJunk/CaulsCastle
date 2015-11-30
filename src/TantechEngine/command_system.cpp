#include "command_system.h"
#include "command_component.h"

namespace te
{
    Command::Command(CommandMask dispatchMask, CommandMask forbidMask, std::function<void(const Entity&, float)> fn)
        : mDispatchMask(dispatchMask)
        , mForbidMask(forbidMask)
        , mFn(fn)
    {}

    Command::~Command() {}

    void Command::execute(const Entity& e, float dt) const
    {
        mFn(e, dt);
    }

    CommandSystem::CommandSystem(std::shared_ptr<CommandComponent> pCommand)
        : mpCommandComponent(pCommand)
        , mCommands()
    {
        if (mpCommandComponent == nullptr)
        {
            throw std::runtime_error("CommandSystem::CommandSystem: Must supply CommandComponent");
        }
    }

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
        mpCommandComponent->forEach([this, dt](const te::Entity& e, CommandInstance inst) {
            std::for_each(std::begin(mCommands), std::end(mCommands), [&](const Command& command){
                if (((command.mDispatchMask & inst.commandMask) == command.mDispatchMask) &&
                    ((command.mForbidMask & inst.commandMask) == 0)) {
                    command.execute(e, dt);
                }
            });
        });
        mCommands.clear();
    }
}
