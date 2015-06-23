#ifndef TE_COMMANDS_H
#define TE_COMMANDS_H

#include <functional>
#include <map>
#include <SDL_keycode.h>
#include <SDL_events.h>

namespace te
{
    class Rectangle;

    enum class Action { UP, DOWN };

    typedef std::function<void()> Command;

    typedef std::map<Action, Command> CommandMap;

    CommandMap createPaddleCommandMap(Rectangle& paddle);

    typedef std::map<std::pair<SDL_Keycode, Uint32>, Action> KeyMap;

    KeyMap createPaddleKeyMap(unsigned int configN = 1);
}

#endif /* TE_COMMANDS_H */
