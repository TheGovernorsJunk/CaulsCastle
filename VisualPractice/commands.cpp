#include "commands.h"
#include "types.h"
#include "entity.h"
#include <SDL_keycode.h>
#include <SDL_events.h>

namespace te
{
    static const float PADDLE_SPEED = 100.f;

    CommandMap createPaddleCommandMap(Rectangle& paddle)
    {
        CommandMap commands;
        commands[Action::UP] = [&]()
        {
            Vector2f vel = paddle.getVelocity() + Vector2f(0.f, -PADDLE_SPEED);
            if (vel.y >= -PADDLE_SPEED && vel.y <= 0.f)
            {
                paddle.setVelocity(vel);
            }
        };
        commands[Action::DOWN] = [&]()
        {
            Vector2f vel = paddle.getVelocity() + Vector2f(0.f, PADDLE_SPEED);
            if (vel.y <= PADDLE_SPEED && vel.y >= 0.f)
            {
                paddle.setVelocity(vel);
            }
        };
        return commands;
    }

    KeyMap createPaddleKeyMap(unsigned int playerN)
    {
        KeyMap keys;
        switch (playerN)
        {
        case 1:
            keys[std::make_pair(SDLK_UP, SDL_KEYDOWN)] = Action::UP;
            keys[std::make_pair(SDLK_DOWN, SDL_KEYDOWN)] = Action::DOWN;
            keys[std::make_pair(SDLK_UP, SDL_KEYUP)] = Action::DOWN;
            keys[std::make_pair(SDLK_DOWN, SDL_KEYUP)] = Action::UP;
            break;
        case 2:
            keys[std::make_pair(SDLK_w, SDL_KEYDOWN)] = Action::UP;
            keys[std::make_pair(SDLK_s, SDL_KEYDOWN)] = Action::DOWN;
            keys[std::make_pair(SDLK_w, SDL_KEYUP)] = Action::DOWN;
            keys[std::make_pair(SDLK_s, SDL_KEYUP)] = Action::UP;
            break;
        default:
            keys[std::make_pair(SDLK_UP, SDL_KEYDOWN)] = Action::UP;
            keys[std::make_pair(SDLK_DOWN, SDL_KEYDOWN)] = Action::DOWN;
            keys[std::make_pair(SDLK_UP, SDL_KEYUP)] = Action::DOWN;
            keys[std::make_pair(SDLK_DOWN, SDL_KEYUP)] = Action::UP;
            break;
        }
        return keys;
    }
}
