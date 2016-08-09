#include "game.h"
#include "game_data.h"
#include "xbox_controller.h"

#include <iostream>

namespace te
{
	static inline void set_button_input(Player_input::Button_state& button_state, decltype(SDL_Event::type) type)
	{
		button_state.fire = type == SDL_JOYBUTTONDOWN;
		button_state.release = type == SDL_JOYBUTTONUP;
	}

	void input_game(Game_data& data, const SDL_Event& evt)
	{
		if (evt.type == SDL_JOYBUTTONDOWN || evt.type == SDL_JOYBUTTONUP) {
			auto player_id = evt.button.which;
			auto& keymap = data.keymaps[player_id];
			auto& input = data.inputs[player_id];

			if (evt.jbutton.button == keymap.dodge) {
				set_button_input(input.dodge, evt.type);
			}
		}
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

		for (auto& input_pair : data.inputs) {
			clear(input_pair.second);
		}
	}

	void draw_game(Game_data& data)
	{
	}
}
