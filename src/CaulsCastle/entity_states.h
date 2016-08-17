#ifndef TE_ENTITY_STATES_H
#define TE_ENTITY_STATES_H

namespace te {

struct Game_data;

enum class State_mode {
	Enter,
	Neutral,
	Exit
};
void step_normal_states(Game_data&);

} // namespace te

#endif
