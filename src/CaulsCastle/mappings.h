#ifndef TE_MAPPINGS_H
#define TE_MAPPINGS_H

#include <SDL.h>

namespace te {

struct Controllermap {
	SDL_GameControllerAxis x_movement;
	SDL_GameControllerAxis y_movement;

	SDL_GameControllerButton dodge;
	SDL_GameControllerButton light_attack;

	Controllermap();
};

struct Keymap {
	SDL_Scancode left;
	SDL_Scancode right;
	SDL_Scancode up;
	SDL_Scancode down;

	SDL_Keycode dodge;

	using Mouse_button = decltype(SDL_MouseButtonEvent::button);
	Mouse_button light_attack;

	Keymap();
};

} // namespace te

#endif
