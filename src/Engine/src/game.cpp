#include <game.h>

void app::Game::MainLoop(void) {
	while (!done()) MainLoopIteration();
}

void app::Game::MainLoopIteration(void) {

	Uint64 counterNow = SDL_GetPerformanceCounter();
	Uint64 difference = (counterNow - lastPerformanceCounter);
	deltaTime = float(difference) / float(performanceFrequency);
	lastPerformanceCounter = counterNow;

	inputSystem.Update();

	renditionSystem.BeginFrame();
	Invoke(render);
	renditionSystem.EndFrame();

	Invoke(input);
	Invoke(anim);
	Invoke(ai);
	Invoke(physics);
	Invoke(collisions);
	Invoke(destruct);
	Invoke(user);
}

bool app::Game::GetKeyDown(SDL_Scancode key) {
	return inputSystem.GetKeyDown(key);
}

void app::Game::InitializeSubsytems(app::Dispatcher *dispatcher, SDL_Window *window) {
	inputSystem.Initialize(dispatcher);
	renditionSystem.Initialize(window);
	audioSystem.Initialize();

	lastPerformanceCounter = SDL_GetPerformanceCounter();
	performanceFrequency = SDL_GetPerformanceFrequency();
}

void app::Game::Clear() {
	audioSystem.Shutdown();
}