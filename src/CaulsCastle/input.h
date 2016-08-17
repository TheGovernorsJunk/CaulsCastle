#ifndef TE_INPUT_H
#define TE_INPUT_H

namespace te {

struct Player_input {
	struct Button_state {
		bool fire;
		bool release;
		Button_state();
	};

	float x_movement;
	float y_movement;

	Button_state dodge;
	Button_state light_attack;
};

void clear(Player_input& input);

} // namespace te

#endif
