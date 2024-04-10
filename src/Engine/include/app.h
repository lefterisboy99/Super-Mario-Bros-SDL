#pragma once

#include <game.h>
#include <common.h>
#include <SDL.h>
#include <event.h>

namespace app {

	class App {
	protected:
		Game *game;

	public:

		explicit App(Game *game);
		~App();

		virtual void Initialize(void) = 0;
		virtual void Load(void) = 0;
		virtual void Run(void) { game->MainLoop(); }
		virtual void RunIteration(void) {game->MainLoopIteration(); }

		Game& GetGame(void) {return *game;}
		virtual void Clear(void) = 0;

		void Main(void) {
			Initialize();
			Load();
			Run();
			Clear();
			game->Clear();
		}

		Dispatcher &GetDispatcher() const {return *eventDispatcher;}

	private:
		SDL_Window *window;

		static constexpr uint16_t Default_Window_Width = 640U;
		static constexpr uint16_t Default_Window_Height = 480U;

		Dispatcher *eventDispatcher;
	};

}