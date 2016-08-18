#include "mappings.h"

namespace te {

Controllermap::Controllermap()
	: x_movement(SDL_CONTROLLER_AXIS_LEFTX)
	, y_movement(SDL_CONTROLLER_AXIS_LEFTY)
	, dodge(SDL_CONTROLLER_BUTTON_B)
	, light_attack(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
{}

Keymap::Keymap()
	: left(SDL_SCANCODE_A)
	, right(SDL_SCANCODE_D)
	, up(SDL_SCANCODE_W)
	, down(SDL_SCANCODE_S)
	, dodge(SDLK_SPACE)
	, light_attack(SDL_BUTTON_LEFT)
{}

} // namespace te
