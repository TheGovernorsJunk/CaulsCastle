#ifndef TE_ENTITY_ANIMATION_H
#define TE_ENTITY_ANIMATION_H

#include "resource_holder.h"
#include "animation.h"
#include <string>

namespace te {

struct Game_data;

struct Entity_animation {
	Resource_id<Animation2> idle;

	Resource_id<Animation2> walk_down;
	Resource_id<Animation2> walk_right;
	Resource_id<Animation2> walk_left;
	Resource_id<Animation2> walk_up;

	Resource_id<Animation2> light_attack_right;
	Resource_id<Animation2> light_attack_left;

	Entity_animation(const std::string& animation_group, Game_data&);
};

} // namespace te

#endif
