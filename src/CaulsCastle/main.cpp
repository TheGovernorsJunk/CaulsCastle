#include "game.h"

#include <SDL.h>
#include <SDL_opengl.h>

#include <iostream>
#include <memory>
#include <cassert>

namespace te
{
	struct SDL_lock {
		SDL_lock(Uint32 flags) {
			auto init = SDL_Init(SDL_INIT_VIDEO);
			assert(init == 0);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			assert(glGetError() != GL_NO_ERROR);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			assert(glGetError() != GL_NO_ERROR);
		}
		~SDL_lock() {
			SDL_Quit();
		}
	};

	class gl_context {
	public:
		gl_context(SDL_Window& w)
			: context{SDL_GL_CreateContext(&w)}
		{
			assert(context != NULL);
		}

		~gl_context()
		{
			delete_context();
		}

		gl_context(gl_context&& rhs) noexcept
			: context{ rhs.context }
		{
			rhs.context = NULL;
		}

		gl_context& operator=(gl_context&& rhs) noexcept
		{
			delete_context();
			context = rhs.context;
			rhs.context = NULL;
			return *this;
		}

		auto get() noexcept
		{
			return context;
		}

	private:
		void delete_context() noexcept
		{
			if (context != NULL) {
				SDL_GL_DeleteContext(context);
			}
		}
		SDL_GLContext context;
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
				 SDL_WINDOW_OPENGL),
		&SDL_DestroyWindow
	};
	auto pWindow = upWindow.get();
	assert(pWindow != NULL);

	gl_context context{ *pWindow };

	glClearColor(0, 0, 0.2f, 1.f);

	auto last_ticks = SDL_GetTicks();
	decltype(last_ticks) time_since_last_update = 0;
	decltype(last_ticks) time_per_frame = 1000 / 60;

	while (true) {
		glClear(GL_COLOR_BUFFER_BIT);

		auto curr_ticks = SDL_GetTicks();
		auto dt = curr_ticks - last_ticks;
		time_since_last_update += dt;

		while (time_since_last_update > time_per_frame) {
			time_since_last_update -= time_per_frame;
			step_game(time_per_frame);
		}

		draw_game();
		SDL_GL_SwapWindow(pWindow);

		last_ticks = curr_ticks;
	}

	return 0;
}
