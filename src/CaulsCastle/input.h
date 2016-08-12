#ifndef TE_INPUT_H
#define TE_INPUT_H

namespace te {

struct Player_input {
	struct Button_state {
		bool fire;
		bool release;
	};

	float x_movement;
	float y_movement;

	Button_state dodge;
};

void clear(Player_input& input);

} // namespace te

#endif
