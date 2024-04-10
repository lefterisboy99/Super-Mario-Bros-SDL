#pragma once
#include <SDL.h>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <color.h>
#include <error.h>

namespace engine {

	class TextureSystem;
	class RenditionSystem;
	/**
	 * Texture.
	 */
	class Texture {
	public:
		Texture() {
			image = 0;
			handle = 0;
			width = height = 0u;
		}
		Texture(uint32_t w, uint32_t h, unsigned char *data, const std::string &path);
		Texture(const Texture &other) {
			image = other.image;
			handle = other.handle;
			width = other.width;
			height = other.height;
			filePath = other.filePath;
		}

		inline uint32_t Width()  const {return width;}
		inline uint32_t Height() const {return height;}
		inline SDL_Texture *Handle() const {return handle;}


		void SetPixel(uint32_t x, uint32_t y, Color color);
		Result<Color> GetPixel(uint32_t x, uint32_t y) const;
		void Access(const SDL_Rect &crop,const std::function<void(Color, uint32_t x, uint32_t y)> &accessor) const;

		void SetDimensions(uint32_t newWidth, uint32_t newHeight);
		bool Apply(const RenditionSystem &rendition);
		bool Allocate();
		void FreeLocal() {
			free(image);
			image = 0;
		}
	private:
		SDL_Texture *handle;
		uint32_t width, height;
		std::string filePath;
		unsigned char *image;
		friend class TextureSystem;
	};

	typedef size_t TextureID;

	/**
	 * TextureSystem
	 * TODO: free textures with SDL on destruction
	 */
	class TextureSystem {
	public:
		Result<Texture *> GetTexture(TextureID id);
		Result<TextureID> LoadTexture(const RenditionSystem &rendition,const std::string &filePath, bool keepCPUCopy = false);
		Result<TextureID> IsTextureLoaded(const std::string &filePath);
	private:
		std::vector<Texture> textures;
		std::map<std::string, TextureID> textureIDs;
	};

}