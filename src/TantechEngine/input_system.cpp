#include "command_system.h"

#include <algorithm>

namespace te
{
    InputSystem::InputSystem(std::shared_ptr<CommandSystem> pCommandSystem)
        : mpCommandSystem(pCommandSystem)
        , mKeyPressMap()
        , mKeyReleaseMap()
    {
        assert(pCommandSystem);
    }

    static void assignCommand(char ch, Command&& cmd, std::map<char, std::vector<Command>>& map)
    {
        auto commandIt = map.find(ch);
        if (commandIt == map.end()) {
            map.insert(std::pair<char, std::vector<Command>>(ch, { std::move(cmd) }));
        } else {
            commandIt->second.push_back(cmd);
        }
    }
    void InputSystem::setKeyPress(char ch, Command&& cmd)
    {
        assignCommand(ch, std::move(cmd), mKeyPressMap);
    }

    void InputSystem::setKeyRelease(char ch, Command&& cmd)
    {
        assignCommand(ch, std::move(cmd), mKeyReleaseMap);
    }

    std::map<char, std::vector<Command>>& InputSystem::getInputMap(InputType type)
    {
        switch (type) {
        case InputType::PRESS:
            return mKeyPressMap;
        case InputType::RELEASE:
            return mKeyReleaseMap;
        default:
            throw std::runtime_error("InputSystem::processInput: Unsupported input type.");
        }
    }

    void InputSystem::processInput(char ch, InputType type)
    {
        std::map<char, std::vector<Command>>& pMap = getInputMap(type);
        auto commandsIt = pMap.find(ch);
        if (commandsIt != pMap.end()) {
            std::for_each(std::begin(commandsIt->second),
                          std::end(commandsIt->second),
                          [this](const Command& cmd) {
                mpCommandSystem->queueCommand(cmd);
            });
        }
    }
}
