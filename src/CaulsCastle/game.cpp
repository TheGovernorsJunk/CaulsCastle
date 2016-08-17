#include "game.h"
#include "game_data.h"
#include "xbox_controller.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <Box2D/Box2D.h>

#include <type_traits>

namespace te {

static inline void set_button_input(Player_input::Button_state& button_state, decltype(SDL_Event::type) type)
{
	button_state.fire = type == SDL_CONTROLLERBUTTONDOWN;
	button_state.release = type == SDL_CONTROLLERBUTTONUP;
}

void input_game(Game_data& data, const SDL_Event& evt)
{
	if (evt.type == SDL_CONTROLLERBUTTONDOWN || evt.type == SDL_CONTROLLERBUTTONUP) {
		const auto player_id = evt.cbutton.which;
		const auto& keymap = data.keymaps[player_id];
		auto& input = data.inputs[player_id];

		if (evt.cbutton.button == keymap.dodge) {
			set_button_input(input.dodge, evt.type);
		}
	}
}

static inline void step_controllers(Game_data& data)
{
	for (const auto& controller_pair : data.controllers) {
		const auto player_id = controller_pair.first;
		const auto& keymap = data.keymaps[player_id];
		auto& input = data.inputs[player_id];

		input.x_movement = SDL_GameControllerGetAxis(controller_pair.second.get(), data.keymaps[player_id].x_movement) / 32767.f;
		if (std::abs(input.x_movement) < 0.3f) input.x_movement = 0;
		input.y_movement = SDL_GameControllerGetAxis(controller_pair.second.get(), data.keymaps[player_id].y_movement) / 32767.f;
		if (std::abs(input.y_movement) < 0.3f) input.y_movement = 0;
	}
}

static inline void step_inputs(Game_data& data)
{
	using Vec = std::remove_reference_t<decltype(data.velocities[0])>;
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
}

static inline void step_velocities(Game_data& data, float dt)
{
	for (auto& velocity_pair : data.velocities) {
		auto found = data.rigid_bodies.find(velocity_pair.first);
		if (found != data.rigid_bodies.end()) {
			found->second->SetLinearVelocity({ velocity_pair.second.x, velocity_pair.second.y });
		}
		else {
			data.positions[velocity_pair.first] += (float)dt * velocity_pair.second;
		}
	}
}

static inline void step_physics_world(Game_data& data, float dt)
{
	data.physics_world->Step(dt, 8, 3);
}

static inline void step_rigid_bodies(Game_data& data)
{
	for (auto& body_pair : data.rigid_bodies) {
		auto position = body_pair.second->GetPosition();
		data.positions[body_pair.first] = { position.x, position.y };
	}
}

static inline void step_animations(Game_data& data, float dt)
{
	for (auto& animation_pair : data.entity_animations2) {
		auto entity_id = animation_pair.first;
		auto& entity_animation = animation_pair.second;
		auto& animation = data.animations2.get(animation_pair.second.id);

		entity_animation.t += dt * animation.delay_unit;
		auto& curr_frame = animation.frames[entity_animation.frame_index];
		if (entity_animation.t > curr_frame.delay) {
			entity_animation.t -= curr_frame.delay;
			entity_animation.frame_index = (entity_animation.frame_index + 1) % animation.frames.size();
		}

		auto found = std::find_if(data.entity_meshes2.begin(), data.entity_meshes2.end(), [entity_id](auto& mesh_pair) {
			return mesh_pair.first == entity_id;
		});
		assert(found != data.entity_meshes2.end());
		found->second.resource_id = animation.frames[entity_animation.frame_index].mesh_id;
	}
}

static inline void set_view(Game_data& data)
{
	auto avatar_found = data.avatars.find(0);
	if (avatar_found != data.avatars.end()) {
		auto position = data.positions[avatar_found->second];
		data.view_matrix = glm::translate(glm::vec3{
			-position.x + data.resolution.x / data.pixel_to_world_scale.x * 0.5f,
			-position.y + data.resolution.y / data.pixel_to_world_scale.y * 0.5f,
			0
		});
	}
}

static inline void clear_inputs(Game_data& data)
{
	for (auto& input_pair : data.inputs) {
		clear(input_pair.second);
	}
}

void step_game(Game_data& data, float dt)
{
	step_controllers(data);
	step_inputs(data);
	step_velocities(data, dt);
	step_physics_world(data, dt);
	step_rigid_bodies(data);
	step_animations(data, dt);
	set_view(data);
	clear_inputs(data);
}

namespace {

template <typename Resource_component>
inline void draw(Game_data& game_data, Resource_component& render_data, const glm::mat4& model_view, const glm::mat4& post_translate)
{
	std::sort(std::begin(render_data), std::end(render_data), [](auto& a, auto& b) {
		return a.second.draw_order < b.second.draw_order;
	});

	for (auto& data_pair : render_data) {
		auto position = game_data.positions[data_pair.first];
		glLoadMatrixf(glm::value_ptr(model_view
					     * glm::translate(glm::vec3{ position.x, position.y, 0 })
					     * post_translate
					     * data_pair.second.transform));
		draw(get_resource(game_data, data_pair.second.resource_id));
	}
}

template <typename Resource>
inline Resource& get_resource(Game_data& game_data, Resource_id<Resource> id);
template <>
inline Mesh2& get_resource(Game_data& game_data, Resource_id<Mesh2> id)
{
	return game_data.meshes2.get(id);
}
template <>
inline Mesh3& get_resource(Game_data& game_data, Resource_id<Mesh3> id)
{
	return game_data.meshes3.get(id);
}

} // namespace

void draw_game(Game_data& data)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, data.resolution.x / data.pixel_to_world_scale.x, data.resolution.y / data.pixel_to_world_scale.y, 0, -10000.0, 10000.0);

	glMatrixMode(GL_MODELVIEW);
	auto pixel_scale = glm::scale(glm::vec3(1 / data.pixel_to_world_scale.x,
					       1 / data.pixel_to_world_scale.y,
					       1));
	draw(data, data.entity_meshes3, data.view_matrix, pixel_scale);
	draw(data, data.entity_meshes2, data.view_matrix, pixel_scale);
}

} // namespace te
