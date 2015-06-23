#include "commands.h"
#include "types.h"
#include "entity.h"
#include <SDL_keycode.h>
#include <SDL_events.h>

namespace te
{
    static const float PADDLE_SPEED = 100.f;

    CommandMap createPaddleCommandMap(std::shared_ptr<Rectangle> pPaddle)
    {
        CommandMap commands;
        if (pPaddle.get() != nullptr)
        {
            commands[Action::UP] = [pPaddle]()
            {
                Vector2f vel = pPaddle.get()->getVelocity() + Vector2f(0.f, -PADDLE_SPEED);
                if (vel.y >= -PADDLE_SPEED && vel.y <= 0.f)
                {
                    pPaddle.get()->setVelocity(vel);
                }
            };
            commands[Action::DOWN] = [pPaddle]()
            {
                Vector2f vel = pPaddle.get()->getVelocity() + Vector2f(0.f, PADDLE_SPEED);
                if (vel.y <= PADDLE_SPEED && vel.y >= 0.f)
                {
                    pPaddle.get()->setVelocity(vel);
                }
            };
        }
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
