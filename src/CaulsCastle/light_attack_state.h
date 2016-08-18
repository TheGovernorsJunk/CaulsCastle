#ifndef TE_LIGHT_ATTACK_STATE_H
#define TE_LIGHT_ATTACK_STATE_H

#include "entity_states.h"
#include "resource_holder.h"
#include "animation.h"
#include <unordered_map>

namespace te {

struct Light_attack_state_record {
	Entity_id id;
	float max_duration;
	float duration;

	Light_attack_state_record(Entity_id id)
		: id{ id }
		, max_duration{ 0.f }
		, duration{ 0.f }
	{}

	bool operator==(const Light_attack_state_record& rhs) const
	{
		return id == rhs.id;
	}
};

class Light_attack_state_table : public State_table<Light_attack_state_table, Light_attack_state_record> {
	friend class State_table<Light_attack_state_table, Light_attack_state_record>;

	void step_entering(Record_type& record, Game_data& data, float dt);
	void step_records(Record_type& record, Game_data& data, float dt);

	std::unordered_map<Resource_id<Animation2>, float> m_animation_durations;
};

} // namespace te

#endif
