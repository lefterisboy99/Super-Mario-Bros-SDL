#include <input_system.h>
#include <SDL.h>
#include <iostream>

namespace engine {

	void InputSystem::Initialize(app::Dispatcher *dispatcher) {
		keyboardState = SDL_GetKeyboardState(nullptr);
		this->dispatcher = dispatcher;
	}

	void InputSystem::Update(void) {

		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: {
					dispatcher->Post(QuitEvent());
				} break;

				case SDL_MOUSEBUTTONDOWN:
				case SDL_MOUSEBUTTONUP: {

					ButtonType type;
					switch (e.button.button) {
						case SDL_BUTTON_LEFT: type = ButtonType::Left; break;
						case SDL_BUTTON_MIDDLE: type = ButtonType::Middle; break;
						case SDL_BUTTON_RIGHT: type = ButtonType::Right; break;
						case SDL_BUTTON_X1: type = ButtonType::X1; break;
						case SDL_BUTTON_X2: type = ButtonType::X2; break;
						default: throw new std::exception("Invalid button type from sdl!");
					}

					ButtonEvent b(e.type == SDL_MOUSEBUTTONDOWN, type);
					dispatcher->Post(b);
				} break;

				case SDL_KEYDOWN:
				case SDL_KEYUP: {
					dispatcher->Post(KeyEvent(e.type == SDL_KEYDOWN, e.key.keysym.scancode));
				} break;
			}
		}

		int x, y;
		SDL_GetMouseState(&x, &y);
		mouseDeltaX = float(x - lastMousePositionX);
		mouseDeltaY = float(y - lastMousePositionY);

		lastMousePositionX = x;
		lastMousePositionY = y;
	}

	bool InputSystem::GetKeyDown(SDL_Scancode scanCode) {
		return keyboardState[scanCode];
	}

	bool InputSystem::GetButtonDown(ButtonType type) {
		return SDL_GetMouseState(0, 0) & SDL_BUTTON((Uint8)type);
	}
}