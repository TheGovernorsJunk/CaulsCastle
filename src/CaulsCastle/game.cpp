#include "game.h"
#include "game_data.h"
#include "xbox_controller.h"

#include <iostream>

namespace te
{
	void input_game(Game_data& data, const SDL_Event& evt)
	{
	}

	void step_game(Game_data& data, decltype(SDL_GetTicks()) dms)
	{
		for (auto& joystick_pair : data.joysticks) {
			auto player_id = joystick_pair.first;
			auto& input = data.inputs[player_id];
			auto& keymap = data.keymaps[player_id];

			input.x_movement = SDL_JoystickGetAxis(joystick_pair.second, data.keymaps[player_id].x_movement) / 32767.f;
			input.y_movement = SDL_JoystickGetAxis(joystick_pair.second, data.keymaps[player_id].y_movement) / 32767.f;
		}
	}

	void draw_game(Game_data& data)
	{
	}
}
