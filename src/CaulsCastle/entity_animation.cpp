#include "entity_animation.h"
#include "game_data.h"
#include <cassert>
#include <array>

namespace te {

static inline bool assign_id(const std::string& type_name,
			     const Animation_group_type_record& record,
			     Game_data& data,
			     Resource_id<Animation2>& id)
{
	if (record.type == type_name) {
		id = get_or_create<Animation2>(record.animation_filename, data);
		return true;
	}
	return false;
}

Entity_animation::Entity_animation(const std::string& animation_group, Game_data& data)
{
	using Pair = std::pair<std::string, Resource_id<Animation2>*>;
	const std::array<Pair, 7> anim_map = {
		Pair{ "idle", &idle },
		Pair{ "walk_down", &walk_down },
		Pair{ "walk_right", &walk_right },
		Pair{ "walk_left", &walk_left },
		Pair{ "walk_up", &walk_up },
		Pair{ "light_attack_right", &light_attack_right },
		Pair{ "light_attack_left", &light_attack_left }
	};

	for (auto& group_type_record : data.animation_group_type_table) {
		if (group_type_record.group_name == animation_group) {
			for (auto& pair : anim_map) {
				if (assign_id(pair.first, group_type_record, data, *pair.second)) {
					break;
				}
			}
		}
	}
}

} // namespace te
