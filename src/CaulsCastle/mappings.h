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

} // namespace te

#endif
