#include <SDL.h>
#include <iostream>
#include <memory>

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

	SDL_lock sdl{SDL_INIT_VIDEO};

	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> upWindow {
		SDL_CreateWindow("Caul's Castle",
				 SDL_WINDOWPOS_CENTERED,
				 SDL_WINDOWPOS_CENTERED,
				 640,
				 480,
				 0),
		&SDL_DestroyWindow
	};
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> upRenderer {
		SDL_CreateRenderer(upWindow.get(),
				   -1,
				   SDL_RENDERER_ACCELERATED),
		&SDL_DestroyRenderer
	};
	auto pRenderer = upRenderer.get();
	SDL_SetRenderDrawColor(pRenderer, 255, 0, 0, 255);

	auto last_ticks = SDL_GetTicks();
	decltype(last_ticks) time_since_last_update = 0;
	decltype(last_ticks) time_per_frame = 1000 / 60;

	while (true) {
		SDL_RenderClear(pRenderer);

		auto curr_ticks = SDL_GetTicks();
		auto dt = curr_ticks - last_ticks;
		time_since_last_update += dt;

		while (time_since_last_update > time_per_frame) {
			time_since_last_update -= time_per_frame;
			std::cout << curr_ticks << std::endl;
		}

		SDL_RenderPresent(pRenderer);

		last_ticks = curr_ticks;
	}

	return 0;
}
