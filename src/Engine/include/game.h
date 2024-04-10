#pragma once

#include <functional>
#include <event.h>
#include <input_system.h>
#include <rendition_system.h>
#include <texture_system.h>
#include <tilemap_system.h>
#include <sprite_system.h>
#include <audio_system.h>
#include <error.h>
#include <SDL.h>

namespace app {

	class Game {
	public:
		using Action 	= std::function<void(Game&)>;
		using Pred 		= std::function<bool(void)>;
		virtual ~Game() {std::cout << "~Game()\n";}

		void Clear();

	private:
		Action render, anim, input, ai, physics, destruct, collisions, user;
		Pred done;

		void Invoke(const Action &f) {
			if (f) f(*this);
		}

		engine::InputSystem inputSystem;
		engine::RenditionSystem renditionSystem;
		engine::TextureSystem textureSystem;
		engine::TilemapSystem tilemapSystem;
		engine::SpriteSystem spriteSystem;
		engine::AudioSystem audioSystem;

		Uint64 performanceFrequency;
		Uint64 lastPerformanceCounter;
		float deltaTime;


	public:
		void SetRender(const Action &f) { render = f;}
		void SetAnim(const Action &f) { anim = f;}
		void SetInput(const Action &f) { input = f;}
		void SetAI(const Action &f) { ai = f;}
		void SetPhysics(const Action &f) { physics = f;}
		void SetDestruct(const Action &f) { destruct = f;}
		void SetCollisions(const Action &f) { collisions = f;}
		void SetUser(const Action &f) { user = f;}

		void MainLoop(void);
		void MainLoopIteration(void);

		void SetIsRunning(Pred isRunningFunc) { done = isRunningFunc; }

		float DeltaTime() const { return deltaTime; }

		bool GetKeyDown(SDL_Scancode key);
		bool GetButtonDown(engine::ButtonType type) {
			return inputSystem.GetButtonDown(type);
		}

		void GetMouseDelta(float &x, float &y) {
			x = inputSystem.mouseDeltaX;
			y = inputSystem.mouseDeltaY;
		}

		inline engine::TilemapSystem& TilemapSystem() { return tilemapSystem; }
		inline engine::RenditionSystem &GetRenderer() {return renditionSystem;}
		inline engine::TextureSystem &GetTextureSystem() {return textureSystem;}
		inline engine::SpriteSystem &SpriteSystem() {return spriteSystem;}
		inline engine::AudioSystem &AudioSystem() { return audioSystem; }

		Result<engine::TextureID> LoadTexture(const std::string &filePath, bool uncommit = false) {
			return textureSystem.LoadTexture(renditionSystem, filePath, uncommit);
		}

		Result<engine::Texture *> GetTexture(engine::TextureID id) {
			return textureSystem.GetTexture(id);
		}


		void InitializeSubsytems(Dispatcher *dispatcher, SDL_Window *window);
	};

}
