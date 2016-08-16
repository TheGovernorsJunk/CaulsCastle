#include "entity_animation.h"
#include "game_data.h"
#include <cassert>

namespace te {

static inline Resource_id<Animation2> get_id(const Animation_group_type_record& record, Game_data& data)
{
	return get_or_create<Animation2>(record.animation_filename, data);
}

Entity_animation::Entity_animation(const std::string& animation_group, Game_data& data)
{
	for (auto& group_type_record : data.animation_group_type_table) {
		if (group_type_record.group_name == animation_group) {
			if (group_type_record.type == "idle") {
				idle = get_id(group_type_record, data);
			}
			else if (group_type_record.type == "walk_down") {
				walk_down = get_id(group_type_record, data);
			}
			else if (group_type_record.type == "walk_right") {
				walk_right = get_id(group_type_record, data);
			}
			else if (group_type_record.type == "walk_left") {
				walk_left = get_id(group_type_record, data);
			}
			else if (group_type_record.type == "walk_up") {
				walk_up = get_id(group_type_record, data);
			}
			else if (group_type_record.type == "light_attack_right") {
				light_attack_right = get_id(group_type_record, data);
			}
			else if (group_type_record.type == "light_attack_left") {
				light_attack_left = get_id(group_type_record, data);
			}
		}
	}
}

} // namespace te
