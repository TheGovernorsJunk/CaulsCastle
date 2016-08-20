#include "light_attack_state.h"
#include "game_data.h"
#include <iostream>

namespace te {

void Light_attack_state_table::step_entering(Record_type& record, Game_data& data, float dt)
{
	const auto& animation_group = data.entity_animation_groups[record.id];
	auto& animation = data.entity_animations2[record.id];

	const auto assign_if = [&animation](bool condition, Resource_id<Animation2> id)
	{
		if (condition) {
			animation.id = id;
			animation.frame_index = 0;
			animation.t = 0;
		}
	};

	auto curr_anim = animation.id;
	assign_if(curr_anim == animation_group.idle_left || curr_anim == animation_group.walk_left,
		  animation_group.light_attack_left);
	assign_if(curr_anim == animation_group.idle_right || curr_anim == animation_group.walk_right,
		  animation_group.light_attack_right);
	assign_if(curr_anim == animation_group.idle_up || curr_anim == animation_group.walk_up,
		  animation_group.light_attack_up);
	assign_if(curr_anim == animation_group.idle_down || curr_anim == animation_group.walk_down,
		  animation_group.light_attack_down);

	auto duration_found = m_animation_durations.find(animation.id);
	if (duration_found == m_animation_durations.end()) {
		auto pair = m_animation_durations.insert(decltype(m_animation_durations)::value_type{
			animation.id,
			get_duration(data.animations2.get(animation.id))
		});
		duration_found = pair.first;
	}
	record.max_duration = duration_found->second;
	record.duration = 0.f;

	data.speeds[record.id] = 0;
}

void Light_attack_state_table::step_records(Record_type& record, Game_data& data, float dt)
{
	record.duration += dt;
	if (record.duration >= record.max_duration) {
		exit_state(record);
		data.normal_state_table.insert(record.id);
	}
}

} // namespace te
