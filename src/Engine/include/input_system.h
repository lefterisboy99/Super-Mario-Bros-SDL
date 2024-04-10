#pragma once

#include <common.h>
#include <SDL.h>
#include <event.h>

namespace engine {
	class QuitEvent : public app::Event {
	public:
		virtual ~QuitEvent() {}
		std::string GetDescription() const { return "Quit"; }
	};

	enum class ButtonType {
		Left = SDL_BUTTON_LEFT,
		Right = SDL_BUTTON_RIGHT,
		Middle = SDL_BUTTON_MIDDLE,
		X1 = SDL_BUTTON_X1,
		X2 = SDL_BUTTON_X2,
	};

	class ButtonEvent : public app::Event {
	public:
		virtual ~ButtonEvent() {}
		std::string GetDescription() const { return "Button"; }

		ButtonEvent(bool pressed, ButtonType type)
			:pressed(pressed), type(type) {}

		ButtonType type;
		bool pressed;
	};

	class KeyEvent : public app::Event {
	public:
		virtual ~KeyEvent() {}
		std::string GetDescription() const { return "Key"; }

		KeyEvent(bool pressed, SDL_Scancode scancode)
			:pressed(pressed), scancode(scancode) {}

		SDL_Scancode scancode;
		bool pressed;
	};

	class InputSystem {
	public:
		void Initialize(app::Dispatcher *dispatcher);
		void Update(void);
		bool GetKeyDown(SDL_Scancode scanCode);
		bool GetButtonDown(ButtonType type);
		float mouseDeltaX = 0.0f;
		float mouseDeltaY = 0.0f;
	private:

		int lastMousePositionX = 0;
		int lastMousePositionY = 0;

		const Uint8 *keyboardState;
		app::Dispatcher *dispatcher;
	};

}