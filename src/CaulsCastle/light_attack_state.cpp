#include "light_attack_state.h"
#include "game_data.h"
#include <iostream>

namespace te {

void Light_attack_state_table::step_entering(Record_type& record, Game_data& data, float dt)
{
	const auto& animation_group = data.entity_animation_groups[record.id];
	auto& animation = data.entity_animations2[record.id];
	auto heading = data.headings[record.id];

	animation.id = animation_group.lookup_table.get(Entity_animation::Query(
		true,
		{},
		{},
		std::abs(heading.x) > std::abs(heading.y),
		heading.x > 0,
		heading.y > 0));
	animation.frame_index = 0;
	animation.t = 0;

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
