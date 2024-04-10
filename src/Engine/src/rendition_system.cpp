#include <rendition_system.h>

#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#define STBTT_STATIC
#define STBRP_STATIC
#include <stb_rect_pack.h>
#include <stb_truetype.h>

namespace engine {

	Result<AsciiFont> AsciiFont::FromFile(SDL_Renderer *const renderer, const char *path, float size) {
		AsciiFont font = {};

		unsigned char *fontBuffer = read_file_and_null_terminate(path);
		ASSERT(fontBuffer);
		font.info = {0};
		font.chars = (stbtt_packedchar*)malloc(sizeof(stbtt_packedchar) * 96);
		ASSERT(font.chars);

		if (stbtt_InitFont(&font.info, fontBuffer, 0) == 0) {
			free(fontBuffer);
			free(font.chars);
			return err("failed to initialize font");
		}

		unsigned char *bitmap = 0;
		font.textureSize = 32;
		while (1) {
			bitmap = (unsigned char *)malloc(font.textureSize * font.textureSize);
			ASSERT(bitmap);
			stbtt_pack_context pack_context;
			stbtt_PackBegin(&pack_context, bitmap, font.textureSize, font.textureSize, 0, 1, 0);
			stbtt_PackSetOversampling(&pack_context, 1, 1);
			if (!stbtt_PackFontRange(&pack_context, fontBuffer, 0, size, 32, 95, font.chars)) {
				// too small
				free(bitmap);
				stbtt_PackEnd(&pack_context);
				font.textureSize *= 2;
			} else {
				stbtt_PackEnd(&pack_context);
				break;
			}
		}

		font.atlas = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STATIC, font.textureSize, font.textureSize);
		ASSERT(font.atlas);
		SDL_SetTextureBlendMode(font.atlas, SDL_BLENDMODE_BLEND);

		uint32_t *pixels = (uint32_t*)malloc(font.textureSize * font.textureSize * sizeof(uint32_t));
		static SDL_PixelFormat* format = NULL;
		if (format == NULL) format = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA32);
		for (uint32_t i = 0; i < font.textureSize * font.textureSize; i++) {
			pixels[i] = SDL_MapRGBA(format, 0xff, 0xff, 0xff, bitmap[i]);
		}
		SDL_UpdateTexture(font.atlas, NULL, pixels, font.textureSize * sizeof(uint32_t));
		free(pixels);
		free(bitmap);

		// setup additional info
		font.scale = stbtt_ScaleForPixelHeight(&font.info, size);
		stbtt_GetFontVMetrics(&font.info, &font.ascent, 0, 0);
		font.baseline = (int) (font.ascent * font.scale);

		free(fontBuffer);
		font.initialized = true;
		return ok(font);
	}


	void RenditionSystem::Initialize(SDL_Window *window) {
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
		ASSERT(renderer);

		currentFont = AsciiFont::FromFile(renderer, "c:\\windows\\fonts\\arial.ttf", 16).unwrap();
	}

	void RenditionSystem::BeginFrame() {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderSetScale(renderer, ZoomLevel, ZoomLevel);
	}

	void RenditionSystem::EndFrame() {
		SDL_RenderPresent(renderer);
		SDL_RenderSetScale(renderer, 1.f, 1.f);
	}

	void RenditionSystem::FillRect(int x, int y, int w, int h) {

		SDL_Rect rect = {x - DefaultView.offset.x, y - DefaultView.offset.y, w, h};
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		SDL_RenderFillRect(renderer, &rect);
	}

 	void RenditionSystem::RenderTexture(SDL_Texture *texture, int x, int y, int w, int h, SDL_Rect *crop) {
		SDL_Rect target = {
			x - DefaultView.offset.x, y - DefaultView.offset.y,
			abs(w), abs(h)
		};

		SDL_RendererFlip flip = SDL_FLIP_NONE;

		if (w < 0) flip = SDL_RendererFlip(flip | SDL_FLIP_HORIZONTAL);
		if (h < 0) flip = SDL_RendererFlip(flip | SDL_FLIP_VERTICAL);

		if (crop) {

			SDL_RenderCopyEx(renderer, texture, crop, &target, 0.0, 0, flip);
		} else {
			SDL_RenderCopyEx(renderer, texture, 0, &target, 0.0, 0, flip);
		}
	}

	void RenditionSystem::RenderRect(int x, int y, int w, int h, Color color) {
		SDL_Rect r = {
			x - DefaultView.offset.x, y - DefaultView.offset.y,
			w, h
		};

		SetColor(color);
		SDL_RenderDrawRect(renderer, &r);
		SetColor();
	}

	void RenditionSystem::Text(const std::string &text,Vec2f position, Color c) {
		position.y += currentFont.baseline;

		// Uint8 r, g, b, a;
		// SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
		SDL_SetTextureColorMod(currentFont.atlas, c.r, c.g, c.b);
		SDL_SetTextureAlphaMod(currentFont.atlas, c.a);

		for (int i = 0; i < text.size(); i++) {
			if (text[i] >= 32 && text[i] < 128) {
				//if(i > 0) x += stbtt_GetCodepointKernAdvance(currentFont.info, text[i - 1], text[i]) * currentFont.scale;

				stbtt_packedchar* info = &currentFont.chars[text[i] - 32];
				SDL_Rect src_rect = {info->x0, info->y0, info->x1 - info->x0, info->y1 - info->y0};
				SDL_Rect dst_rect = {(int)position.x + (int)info->xoff, (int)position.y + (int)info->yoff, info->x1 - info->x0, info->y1 - info->y0};

				SDL_RenderCopy(renderer, currentFont.atlas, &src_rect, &dst_rect);
				position.x += info->xadvance;
			}
		}

	}

}