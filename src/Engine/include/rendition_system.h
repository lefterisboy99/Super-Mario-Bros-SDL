#pragma once

#include <common.h>
#include <SDL.h>
#include <color.h>
#include <stb_truetype.h>
#include <texture_system.h>
#include <math2d.h>
#include <error.h>

namespace engine {

	/**
	 * Ascii font info structure for rendition; basically for debug utilities
	 */
	class AsciiFont {
	public:
		AsciiFont() = default;
		static Result<AsciiFont> FromFile(SDL_Renderer *const renderer, const char *path, float size);
		stbtt_fontinfo info;
		stbtt_packedchar *chars;
		SDL_Texture *atlas;
		uint32_t textureSize;
		float size;
		float scale;
		int ascent;
		int baseline;
		bool initialized = false;
	};

	class View {
	public:
		Vec2i offset = {};
	};

	class RenditionSystem {
	public:
		void Initialize(SDL_Window *window);

		void BeginFrame();
		void EndFrame();
		void FillRect(int x, int y, int w, int h);

		SDL_Texture *CreateTextureFromImage(SDL_Surface *surface) const {
			ASSERT(surface);
			ASSERT(renderer);
			return SDL_CreateTextureFromSurface(renderer, surface);
		}

		void RenderTexture(SDL_Texture *texture, int x, int y, int w, int h, SDL_Rect *crop = 0);
		void RenderRect(int x, int y, int w, int h, Color color = Color::Red());
		void Text(const std::string &text,Vec2f position, Color c = Color::White());

		Recti GetViewBox() const {
			int w, h;
			SDL_GetRendererOutputSize(renderer, &w, &h);

			return Recti(DefaultView.offset, Vec2i{w, h});
		}

		float ZoomLevel = 1.0f;
		View DefaultView;
	private:
		SDL_Renderer *renderer;
		void SetColor(Color color = Color::Black()) { SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);}

		AsciiFont currentFont;
	};

}