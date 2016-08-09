#ifndef TE_GAME_DATA_H
#define TE_GAME_DATA_H

#include "types.h"
#include "xbox_controller.h"

#include <boost/container/flat_map.hpp>

#include <memory>

namespace te
{
	struct Keymap {
		Xbox_axis x_movement;
		Xbox_axis y_movement;

		Keymap()
			: x_movement{ X_axis }
			, y_movement{ Y_axis }
		{}
	};

	struct Player_input {
		float x_movement;
		float y_movement;
	};

	struct Game_data {
		entity_id next_id;

		flat_map<Player_id, SDL_Joystick*> joysticks;
		flat_map<Player_id, Keymap> keymaps;
		flat_map<Player_id, Player_input> inputs;

		component<entity_id, vec2> positions;

		Game_data();
	};
}

#endif
