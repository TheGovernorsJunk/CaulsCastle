#include "input.h"

namespace te {

Player_input::Button_state::Button_state()
	: fire{ false }
	, release{ false }
{}

static inline void clear(Player_input::Button_state& state)
{
	state.fire = false;
	state.release = false;
}

void clear(Player_input& input)
{
	input.x_movement = 0;
	input.y_movement = 0;

	clear(input.dodge);
	clear(input.light_attack);
}

} // namespace te
