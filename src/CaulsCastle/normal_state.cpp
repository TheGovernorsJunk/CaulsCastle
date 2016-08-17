#include "entity_states.h"
#include "game_data.h"
#include <algorithm>

namespace te {

static inline void step_inputs(Game_data& data)
{
	for (auto& input_pair : data.inputs) {
		const auto player_id = input_pair.first;
		const auto& input = input_pair.second;
		const auto entity_id = data.avatars[player_id];

		if (data.normal_state_table.find(entity_id) != data.normal_state_table.end()) {
			using Vec = std::remove_reference_t<decltype(data.velocities[0])>;

			auto max_speed = data.max_speeds[entity_id];
			data.velocities[entity_id] = max_speed * Vec{
				input.x_movement,
				input.y_movement
			};

			if (input.light_attack.fire) {
				data.light_attack_state_table[entity_id] = { State_mode::Enter };
				data.normal_state_table.erase(entity_id);
			}
		}
	}
}

static inline void step_animations(Game_data& data)
{
	for (const auto record_pair : data.normal_state_table) {
		if (record_pair.second.mode != State_mode::Neutral) continue;

		const auto entity_id = record_pair.first;
		const auto& group = data.entity_animation_groups[entity_id];
		const auto velocity = data.velocities[entity_id];
		auto& animation = data.entity_animations2[entity_id];

		const auto assign_if = [&animation](bool condition,
						    Resource_id<Animation2> animation_id)
		{
			if (condition && animation.id != animation_id) {
				animation.id = animation_id;
				animation.frame_index = 0;
				animation.t = 0;
			}
		};

		auto x_mag = std::abs(velocity.x);
		auto y_mag = std::abs(velocity.y);
		if (x_mag > y_mag) {
			assign_if(velocity.x > 0, group.walk_right);
			assign_if(velocity.x < 0, group.walk_left);
		}
		else if (y_mag > x_mag) {
			assign_if(velocity.y > 0, group.walk_down);
			assign_if(velocity.y < 0, group.walk_up);
		}
		else {
			assign_if(animation.id == group.walk_right, group.idle_right);
			assign_if(animation.id == group.walk_left, group.idle_left);
			assign_if(animation.id == group.walk_down, group.idle_down);
			assign_if(animation.id == group.walk_up, group.idle_up);
		}
	}
}

void step_normal_states(Game_data& data)
{
	step_inputs(data);
	step_animations(data);
}

} // namespace te

