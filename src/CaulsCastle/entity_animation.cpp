#include "entity_animation.h"
#include "game_data.h"
#include <cassert>
#include <array>

namespace te {

Entity_animation::Entity_animation(const std::string& animation_group, Game_data& data)
{
	using Pair = std::pair<std::string, Resource_id<Animation2>*>;
	const std::array<Pair, 10> anim_map = {
		Pair{ "idle_down", &idle_down },
		Pair{ "idle_right", &idle_right },
		Pair{ "idle_left", &idle_left },
		Pair{ "idle_up", &idle_up },
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
				if (group_type_record.type == pair.first) {
					*pair.second = get_or_create<Animation2>(group_type_record.animation_filename, data);
					break;
				}
			}
		}
	}
}

} // namespace te
