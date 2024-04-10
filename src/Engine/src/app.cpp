#include <app.h>
#include <exception>

_internal bool SDL_Is_Initialized = false;
_internal bool SDL_Exit_Requested = false;

namespace app {
	void OnApplicationQuit(App &app, const Event &event) {
		SDL_Exit_Requested = true;
		SDL_Quit();
	}

	bool GameIsRunning(void) {
		return SDL_Exit_Requested;
	}

	App::App(Game *game) {
		this->game = game;
		if (!SDL_Is_Initialized) {
			if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0) {
				throw new std::exception("Runtime initialization error of SDL");
			}

			SDL_Is_Initialized = true;
		}

		window = SDL_CreateWindow(
			"Sample Title",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			Default_Window_Width, Default_Window_Height,
			SDL_WINDOW_SHOWN);
		if (!window) throw new std::exception("Could not create window!");

		eventDispatcher = new Dispatcher(this);
		eventDispatcher->Register("Quit", OnApplicationQuit);

		game->InitializeSubsytems(eventDispatcher, window);

		game->SetIsRunning(GameIsRunning);
	}

	App::~App() {
		SDL_DestroyWindow(window);
	}
}