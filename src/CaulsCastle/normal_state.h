#ifndef TE_NORMAL_STATE_H
#define TE_NORMAL_STATE_H

#include "entity_states.h"

namespace te {

class Normal_state_table : public State_table<Normal_state_table> {
	friend class State_table<Normal_state_table>;

	void step_input(Entity_id id, Game_data& data);
	void step_animation(Entity_id entity_id, Game_data& data);

	void step_records(Entity_id id, Game_data& data, float dt)
	{
		step_input(id, data);
		step_animation(id, data);
	}
};

} // namespace te

#endif
