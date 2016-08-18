#include "mappings.h"

namespace te {

Controllermap::Controllermap()
	: x_movement(SDL_CONTROLLER_AXIS_LEFTX)
	, y_movement(SDL_CONTROLLER_AXIS_LEFTY)
	, dodge(SDL_CONTROLLER_BUTTON_B)
	, light_attack(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
{}

} // namespace te
