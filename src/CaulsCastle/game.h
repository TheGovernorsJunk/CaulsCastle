#ifndef TE_GAME_H
#define TE_GAME_H

#include <SDL.h>

namespace te
{
	void step_game(decltype(SDL_GetTicks()) dms);
	void draw_game(SDL_Renderer& renderer);
}

#endif
