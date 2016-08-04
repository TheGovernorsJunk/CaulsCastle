#ifndef TE_GAME_H
#define TE_GAME_H

#include <SDL.h>

namespace te
{
	void step_game(SDL_Renderer&, decltype(SDL_GetTicks()) dms);
}

#endif
