#include <SDL.h>
#include <iostream>

namespace te
{
	struct SDL_lock
	{
		SDL_lock(Uint32 flags) {
			if (SDL_Init(SDL_INIT_VIDEO) != 0) {
				std::cerr << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
			}
		}
		~SDL_lock() {
			SDL_Quit();
		}
	};
}

int main(int argc, char** argv)
{
	using namespace te;
	SDL_lock sdl(SDL_INIT_VIDEO);
	return 0;
}
