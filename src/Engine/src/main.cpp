#include <SDL.h>
#include "common.h"

_global SDL_Window 		*g_window;
_global bool 			g_running = true;
_global SDL_Renderer 	*g_renderer;
_global const Uint8 	*g_keyboard_state;

_inline bool get_key_down(SDL_Scancode scancode) {
	return g_keyboard_state[scancode];
}

int main(int argc, char *argv[]) {
	UNUSED(argc);
	UNUSED(argv);

	if (SDL_Init(SDL_INIT_VIDEO)) {

		return -1;
	}

	g_window = SDL_CreateWindow("hy454", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 640u, 480u,SDL_WINDOW_SHOWN);
	ASSERT(g_window);

	g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	ASSERT(g_renderer);

	g_keyboard_state = SDL_GetKeyboardState(0);

	while (g_running) {

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: {
					g_running = false;
				} break;

				case SDL_KEYUP: {
					if (e.key.keysym.scancode == SDL_SCANCODE_D) {
						SDL_Log("Pressed!");
					}
				} break;
			}
		}

		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

		SDL_RenderClear(g_renderer);

		_persist float x = 0.f;

		SDL_FRect my_rect = {
			x,10.f,
			64.f,64.f
		};

		if (get_key_down(SDL_SCANCODE_A)) {
			SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 255);

			x += 0.1f;
		} else {	
			SDL_SetRenderDrawColor(g_renderer, 255, 255, 0, 255);
		}

		SDL_RenderFillRectF(g_renderer, &my_rect);

		SDL_RenderPresent(g_renderer);
	}

	SDL_Quit();

	return 0;
}