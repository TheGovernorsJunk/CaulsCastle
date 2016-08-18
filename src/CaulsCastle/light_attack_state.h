#ifndef TE_LIGHT_ATTACK_STATE_H
#define TE_LIGHT_ATTACK_STATE_H

#include "entity_states.h"

namespace te {

class Light_attack_state_table : public State_table<Light_attack_state_table> {
	friend class State_table<Light_attack_state_table>;

	void step_entering(Entity_id entity_id, Game_data& data, float dt);
	void step_records(Entity_id entity_id, Game_data& data, float dt);
};

} // namespace te

#endif
