#include "game.h"
#include "game_data.h"
#include "xbox_controller.h"

#include <glm/gtc/type_ptr.hpp>
#include <Box2D/Box2D.h>

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

	void step_game(Game_data& data, float dt)
	{
		using Vec = std::remove_reference_t<decltype(data.velocities[0])>;

		for (auto& controller_pair : data.controllers) {
			const auto player_id = controller_pair.first;
			const auto& keymap = data.keymaps[player_id];
			auto& input = data.inputs[player_id];

			input.x_movement = SDL_GameControllerGetAxis(controller_pair.second.get(), data.keymaps[player_id].x_movement) / 32767.f;
			if (std::abs(input.x_movement) < 0.3f) input.x_movement = 0;
			input.y_movement = SDL_GameControllerGetAxis(controller_pair.second.get(), data.keymaps[player_id].y_movement) / 32767.f;
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
			auto found = data.rigid_bodies.find(velocity_pair.first);
			if (found != data.rigid_bodies.end()) {
				found->second->SetLinearVelocity({ velocity_pair.second.x, velocity_pair.second.y });
			}
			else {
				data.positions[velocity_pair.first] += (float)dt * velocity_pair.second;
			}
		}
		data.physics_world->Step(dt, 8, 3);
		for (auto& body_pair : data.rigid_bodies) {
			auto position = body_pair.second->GetPosition();
			data.positions[body_pair.first] = { position.x, position.y };
		}

		for (auto& input_pair : data.inputs) {
			clear(input_pair.second);
		}
	}

	namespace {
		template <typename Resource_component>
		inline void draw(Game_data& game_data, Resource_component& render_data)
		{
			for (auto& data_pair : render_data) {
				glLoadMatrixf(glm::value_ptr(game_data.view_matrix));
				auto position = game_data.positions[data_pair.first];
				glTranslatef(position.x, position.y, 0);

				glMultMatrixf(glm::value_ptr(data_pair.second.transform));
				draw(get_resource(game_data, data_pair.second.resource_id));
			}
		}

		template <typename Resource>
		inline Resource& get_resource(Game_data& game_data, Resource_id<Resource> id);
		template <>
		inline Mesh<vec3, vec2>& get_resource(Game_data& game_data, Resource_id<Mesh<vec3, vec2>> id)
		{
			return game_data.meshes3.get(id);
		}
	}

	void draw_game(Game_data& data)
	{
		glMatrixMode(GL_MODELVIEW);
		draw(data, data.entity_meshes3);
	}
}
