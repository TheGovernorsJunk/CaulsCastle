#ifndef TE_COMMAND_SYSTEM_H
#define TE_COMMAND_SYSTEM_H

#include "typedefs.h"
#include "ecs.h"

#include <functional>
#include <memory>
#include <vector>
#include <map>

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

    enum class InputType
    { PRESS, RELEASE };

    class InputSystem
    {
    public:
        InputSystem(std::shared_ptr<CommandSystem>);
        void setKeyPress(char, Command&&);
        void setKeyRelease(char, Command&&);
        void processInput(char, InputType);
    private:
        std::map<char, std::vector<Command>>& getInputMap(InputType);

        std::shared_ptr<CommandSystem> mpCommandSystem;

        std::map<char, std::vector<Command>> mKeyPressMap;
        std::map<char, std::vector<Command>> mKeyReleaseMap;
    };
}

#endif
