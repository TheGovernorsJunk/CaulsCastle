#ifndef TE_GAME_H
#define TE_GAME_H

#include <SDL.h>

namespace te
{
	struct Game_data;

	void input_game(Game_data& data, const SDL_Event&);
	void step_game(Game_data& data, float dms);
	void draw_game(Game_data& data);
}

#endif
