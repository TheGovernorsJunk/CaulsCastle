#include "input.h"

namespace te {

void clear(Player_input& input)
{
	input.x_movement = 0;
	input.y_movement = 0;

	input.dodge.fire = false;
	input.dodge.release = false;
}

} // namespace te
