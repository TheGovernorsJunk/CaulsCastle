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
	button_state.fire = (type == SDL_CONTROLLERBUTTONDOWN)
		|| (type == SDL_KEYDOWN)
		|| (type == SDL_MOUSEBUTTONDOWN);
	button_state.release = (type == SDL_CONTROLLERBUTTONUP)
		|| (type == SDL_KEYUP)
		|| (type == SDL_MOUSEBUTTONUP);
}

void input_game(Game_data& data, const SDL_Event& evt)
{
	if (evt.type == SDL_CONTROLLERBUTTONDOWN || evt.type == SDL_CONTROLLERBUTTONUP) {
		const auto player_id = evt.cbutton.which;
		const auto& controllermap = data.controllermaps[player_id];
		auto& input = data.inputs[player_id];

		if (evt.cbutton.button == controllermap.dodge) {
			set_button_input(input.dodge, evt.type);
		}
		else if (evt.cbutton.button == controllermap.light_attack) {
			set_button_input(input.light_attack, evt.type);
		}
	}
	else if (data.controllers.find(0) == data.controllers.end()) {
		const auto& keymap = data.keymaps[0];
		auto& input = data.inputs[0];

		if (evt.type == SDL_KEYDOWN || evt.type == SDL_KEYUP) {
			if (evt.key.keysym.sym == keymap.dodge) {
				set_button_input(input.dodge, evt.type);
			}
		}
		else if (evt.type == SDL_MOUSEBUTTONDOWN || evt.type == SDL_MOUSEBUTTONUP) {
			if (evt.button.button == keymap.light_attack) {
				set_button_input(input.light_attack, evt.type);
			}
		}
	}
}

static inline void step_controllers(Game_data& data)
{
	for (const auto& controller_pair : data.controllers) {
		const auto player_id = controller_pair.first;
		const auto& controllermap = data.controllermaps[player_id];
		auto& input = data.inputs[player_id];

		input.x_movement = SDL_GameControllerGetAxis(controller_pair.second.get(), data.controllermaps[player_id].x_movement) / 32767.f;
		if (std::abs(input.x_movement) < 0.3f) input.x_movement = 0;
		input.y_movement = SDL_GameControllerGetAxis(controller_pair.second.get(), data.controllermaps[player_id].y_movement) / 32767.f;
		if (std::abs(input.y_movement) < 0.3f) input.y_movement = 0;
	}
}

static inline void step_keyboard(Game_data& data)
{
	if (data.controllers.find(0) != data.controllers.end()) {
		return;
	}

	const auto& keymap = data.keymaps[0];
	auto& input = data.inputs[0];
	const auto* key_states = SDL_GetKeyboardState(NULL);

	input.x_movement = (float)key_states[keymap.right] - key_states[keymap.left];
	input.y_movement = (float)key_states[keymap.down] - key_states[keymap.up];
	if (input.x_movement != 0 && input.y_movement != 0) {
		auto normalized_movement = glm::normalize(glm::vec2{ input.x_movement, input.y_movement });
		input.x_movement = normalized_movement.x;
		input.y_movement = normalized_movement.y;
	}
}

static inline void step_velocities(Game_data& data, float dt)
{
	for (auto& heading_pair : data.headings) {
		const auto entity_id = heading_pair.first;
		const auto speed = data.speeds[entity_id];
		const auto velocity = speed * heading_pair.second;

		auto found = data.physics_manager.find_rigid_body(entity_id);
		if (found != data.physics_manager.end()) {
			found->second->SetLinearVelocity({ velocity.x, velocity.y });
		}
		else {
			data.positions[entity_id] += (float)dt * velocity;
		}
	}
}

static inline void step_physics_world(Game_data& data, float dt)
{
	data.physics_manager.step(dt);
}

static inline void step_rigid_bodies(Game_data& data)
{
	for (auto& body_pair : data.physics_manager) {
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

static inline void step_attack_queries(Game_data& data)
{
	struct Callback : public b2QueryCallback {
		Entity_id attacker_id;
		Callback(Entity_id attacker_id) : attacker_id{ attacker_id } {}
		bool ReportFixture(b2Fixture* fixture) override
		{
			Entity_id id = *static_cast<Entity_id*>(fixture->GetBody()->GetUserData());
			if (id != attacker_id && fixture->IsSensor()) {
				// Handle attack
			}
			return true;
		}
	};

	for (const auto& query : data.attack_queries) {
		Callback query_callback{ query.entity_id };
		data.physics_manager.query_aabb(query_callback, query.aabb);
	}
	data.attack_queries.clear();
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
	step_keyboard(data);
	data.normal_state_table.step(data, dt);
	data.light_attack_state_table.step(data, dt);
	step_velocities(data, dt);
	step_physics_world(data, dt);
	step_rigid_bodies(data);
	step_animations(data, dt);
	step_attack_queries(data);
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
