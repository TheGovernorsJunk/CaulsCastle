#include "light_attack_state.h"
#include "game_data.h"
#include <iostream>

namespace te {

void Light_attack_state_table::step_entering(Entity_id entity_id, Game_data& data, float dt)
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

void Light_attack_state_table::step_records(Entity_id entity_id, Game_data& data, float dt)
{
}

} // namespace te
