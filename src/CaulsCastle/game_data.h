#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include "types.h"
#include "xbox_controller.h"
#include "input.h"

#include <boost/container/flat_map.hpp>

#include <memory>

namespace te
{
	struct Keymap {
		Xbox_axis x_movement;
		Xbox_axis y_movement;

		Xbox_button dodge;

		Keymap()
			: x_movement{ X_axis }
			, y_movement{ Y_axis }
			, dodge{ B_button }
		{}
	};

	struct Game_data {
		Entity_id next_id;

		flat_map<Player_id, SDL_Joystick*> joysticks;
		flat_map<Player_id, Keymap> keymaps;
		flat_map<Player_id, Player_input> inputs;
		flat_map<Player_id, Entity_id> avatars;

		component<Entity_id, float> max_speeds;
		component<Entity_id, vec2> velocities;
		component<Entity_id, vec2> positions;

		Game_data();
	};
}

#endif
