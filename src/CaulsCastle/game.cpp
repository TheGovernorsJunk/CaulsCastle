#include "game.h"
#include "game_data.h"
#include "xbox_controller.h"

#include <glm/gtc/type_ptr.hpp>

#include <type_traits>

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
			const auto player_id = evt.jbutton.which;
			const auto& keymap = data.keymaps[player_id];
			auto& input = data.inputs[player_id];

			if (evt.jbutton.button == keymap.dodge) {
				set_button_input(input.dodge, evt.type);
			}
		}
	}

	void step_game(Game_data& data, float dms)
	{
		using Vec = std::remove_reference_t<decltype(data.velocities[0])>;

		for (auto& joystick_pair : data.joysticks) {
			const auto player_id = joystick_pair.first;
			const auto& keymap = data.keymaps[player_id];
			auto& input = data.inputs[player_id];

			input.x_movement = SDL_JoystickGetAxis(joystick_pair.second, data.keymaps[player_id].x_movement) / 32767.f;
			if (std::abs(input.x_movement) < 0.3f) input.x_movement = 0;
			input.y_movement = SDL_JoystickGetAxis(joystick_pair.second, data.keymaps[player_id].y_movement) / 32767.f;
			if (std::abs(input.y_movement) < 0.3f) input.y_movement = 0;
		}

		for (auto& input_pair : data.inputs) {
			const auto player_id = input_pair.first;
			const auto& input = input_pair.second;
			const auto entity_id = data.avatars[player_id];

			auto max_speed = data.max_speeds[entity_id];
			data.velocities[entity_id] = max_speed * Vec{
				input.x_movement,
				input.y_movement
			};
		}

		for (auto& velocity_pair : data.velocities) {
			data.positions[velocity_pair.first] += (float)dms * velocity_pair.second;
		}

		for (auto& input_pair : data.inputs) {
			clear(input_pair.second);
		}
	}

	void draw_game(Game_data& data)
	{
		glMatrixMode(GL_MODELVIEW);
		for (auto& mesh_pair : data.meshes3) {
			glLoadMatrixf(glm::value_ptr(data.view_matrix));
			auto position = data.positions[mesh_pair.first];
			glTranslatef(position.x, position.y, 0);
			draw(mesh_pair.second);
		}
	}
}
