#include "normal_state.h"
#include "game_data.h"
#include <algorithm>

namespace te {

void Normal_state_table::step_entering(Entity_id entity_id, Game_data& data, float dt)
{
	const auto heading = data.headings[entity_id];
	const auto x_mag = std::abs(heading.x);
	const auto y_mag = std::abs(heading.y);
	const auto& animation_group = data.entity_animation_groups[entity_id];
	auto& animator = data.entity_animations2[entity_id];

	animator.id = animation_group.lookup_table.get(
		Entity_animation::Query{ false, false, {}, x_mag > y_mag, heading.x > 0, heading.y > 0 }
	);
	animator.frame_index = 0;
	animator.t = 0;
}

void Normal_state_table::step_input(Entity_id entity_id, Game_data& data)
{
	for (auto& input_pair : data.inputs) {
		const auto player_id = input_pair.first;
		const auto& input = input_pair.second;
		const auto player_entity_id = data.avatars[player_id];

		if (player_entity_id == entity_id) {
			auto max_speed = data.max_speeds[entity_id];
			auto input_mag = glm::length(glm::vec2{ input.x_movement, input.y_movement });
			const auto speed = max_speed * input_mag;
			data.speeds[entity_id] = speed;
			if (speed > 0) {
				data.headings[entity_id] = glm::vec2{
					input.x_movement,
					input.y_movement
				} / input_mag;
			}

			if (input.light_attack.fire) {
				data.light_attack_state_table.insert({ entity_id });
				exit_state(entity_id);
			}
		}
	}
}

void Normal_state_table::step_animation(Entity_id entity_id, Game_data& data)
{
	const auto& group = data.entity_animation_groups[entity_id];
	const auto speed = data.speeds[entity_id];
	const auto heading = data.headings[entity_id];
	auto& animation = data.entity_animations2[entity_id];

	const auto x_mag = std::abs(heading.x);
	const auto y_mag = std::abs(heading.y);

	auto new_id = group.lookup_table.get(Entity_animation::Query(
		false,
		speed > 0,
		false,
		x_mag > y_mag,
		heading.x > 0,
		heading.y > 0));
	if (new_id != animation.id) {
		animation.id = new_id;
		animation.frame_index = 0;
		animation.t = 0;
	}
}

void Normal_state_table::step_records(Entity_id id, Game_data& data, float dt)
{
	step_input(id, data);
	step_animation(id, data);
}

} // namespace te

