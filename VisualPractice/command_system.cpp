#include "command_system.h"
#include "command_component.h"

namespace te
{
    Command::Command(int dispatchMask, std::function<void(const Entity&, float)> fn)
        : mDispatchMask(dispatchMask)
        , mFn(fn)
    {}

    Command::~Command() {}

    void Command::execute(const Entity& e, float dt) const
    {
        mFn(e, dt);
    }

    CommandSystem::CommandSystem(std::shared_ptr<CommandComponent> pCommand)
        : mpCommandComponent(pCommand)
        , mCommands() {}

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
                if ((command.mDispatchMask & inst.typeMask) == command.mDispatchMask) {
                    command.execute(e, dt);
                }
            });
        });
        mCommands.clear();
    }
}
