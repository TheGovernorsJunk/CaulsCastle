#ifndef TE_ENTITY_ANIMATION_H
#define TE_ENTITY_ANIMATION_H

#include "resource_holder.h"
#include "animation.h"
#include "condition_table.h"
#include "tri_bool.h"
#include <string>

namespace te {

struct Game_data;

struct Entity_animation {
	Resource_id<Animation2> idle_down;
	Resource_id<Animation2> idle_right;
	Resource_id<Animation2> idle_left;
	Resource_id<Animation2> idle_up;

	Resource_id<Animation2> walk_down;
	Resource_id<Animation2> walk_right;
	Resource_id<Animation2> walk_left;
	Resource_id<Animation2> walk_up;

	Resource_id<Animation2> light_attack_down;
	Resource_id<Animation2> light_attack_right;
	Resource_id<Animation2> light_attack_left;
	Resource_id<Animation2> light_attack_up;

	struct Query {
		Tri_bool is_light_attacking;
		Tri_bool is_moving;
		Tri_bool high_speed;
		Tri_bool mag_x_gt_mag_y;
		Tri_bool x_is_positive;
		Tri_bool y_is_positive;

		constexpr Query(Tri_bool is_light_attacking,
				Tri_bool is_moving,
				Tri_bool high_speed,
				Tri_bool mag_x_gt_mag_y,
				Tri_bool x_is_positive,
				Tri_bool y_is_positive)
			: is_light_attacking(is_light_attacking)
			, is_moving(is_moving)
			, high_speed(high_speed)
			, mag_x_gt_mag_y(mag_x_gt_mag_y)
			, x_is_positive(x_is_positive)
			, y_is_positive(y_is_positive)
		{}

		constexpr bool operator==(const Query& rhs) const noexcept
		{
			return is_light_attacking == rhs.is_light_attacking
				&& is_moving == rhs.is_moving
				&& high_speed == rhs.high_speed
				&& mag_x_gt_mag_y == rhs.mag_x_gt_mag_y
				&& x_is_positive == rhs.x_is_positive
				&& y_is_positive == rhs.y_is_positive;
		}
	};
	Condition_table<Query, Resource_id<Animation2>> lookup_table;

	Entity_animation(const std::string& animation_group, Game_data&);
	Entity_animation() = default;
};

} // namespace te

#endif
