#include "entity_states.h"
#include "game_data.h"
#include <iostream>

namespace te {

static inline void enter_state(Entity_id entity_id, Game_data& data)
{
	const auto& animation_group = data.entity_animation_groups[entity_id];
	auto& animation = data.entity_animations2[entity_id];

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
}

void step_light_attack_states(Game_data& data)
{
	for (auto& record_pair : data.light_attack_state_table) {
		const auto entity_id = record_pair.first;
		switch (record_pair.second.mode) {
		case State_mode::Enter:
			enter_state(entity_id, data);
			record_pair.second.mode = State_mode::Neutral;
			break;
		case State_mode::Neutral:
			break;
		default:
			assert(false);
		}
	}
}

} // namespace te
