#include <texture_system.h>
#include <rendition_system.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace engine {


	Texture::Texture(uint32_t w, uint32_t h, unsigned char *data, const std::string &path) {
		image = data;
		handle = 0;
		width = w;
		height = h;
		filePath = path;
	}

	void Texture::SetPixel(uint32_t x, uint32_t y, Color color) {
		if (image) {
			((uint32_t*)image)[y * Width() + x] = color.dword;
		}
	}

	void Texture::Access(const SDL_Rect &crop,const std::function<void(Color, uint32_t x, uint32_t y)> &accessor) const {
		uint32_t startX = crop.x; uint32_t endX = crop.x + crop.w;
		uint32_t startY = crop.y; uint32_t endY = crop.y + crop.h;

		ASSERT(startX >= 0 && startY >= 0);
		ASSERT(endX <= width && endY <= height);

		for (uint32_t y = startY; y != endY; ++y) {
			for (uint32_t x = startX; x != endX; ++x) {
				accessor(GetPixel(x,y).unwrap(), x, y);
			}
		}
	}

	void Texture::SetDimensions(uint32_t newWidth, uint32_t newHeight) {
		size_t newSize = width * height * sizeof(uint32_t);
		void *newP = realloc(image, newSize);

		if (newP != 0) image = (unsigned char*)newP;
	}

	Result<Color> Texture::GetPixel(uint32_t x, uint32_t y) const {
		if (image) {
			return ok(Color(((uint32_t*)image)[y * Width() + x]));
		}
		return err("Texture doesn't keep local cpu copy");
	}

	bool Texture::Apply(const RenditionSystem &rendition) {
		const uint32_t mask_red = 0x000000ff;
		const uint32_t mask_green = 0x0000ff00;
		const uint32_t mask_blue = 0x00ff0000;
		const uint32_t mask_alpha = 0xff000000;
		SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void*)image, width, height, 32, 4*width, mask_red, mask_green, mask_blue, mask_alpha);

		if (!surface)
			return false;

		handle = rendition.CreateTextureFromImage(surface);
		if (!handle) {
			SDL_FreeSurface(surface);
			return false;
		}

		SDL_FreeSurface(surface);
		return true;
	}

	Result<TextureID> TextureSystem::LoadTexture(const RenditionSystem &rendition,const std::string &filePath, bool keepCPUCopy) {
		TextureID result;

		// if already loaded, just return the existing texture id
		auto loadedTexture = IsTextureLoaded(filePath);
		if (loadedTexture.good()) {
			return loadedTexture;
		}

		// load otherwise
		int x, y, n;
		unsigned char *data = stbi_load(filePath.c_str(), &x, &y, &n, 4);
		if (!data) {
			return err("Could not load texture for specified path");
		}

		Texture newTexture(x,y,data,filePath);

		if (!newTexture.Apply(rendition)) {
			newTexture.FreeLocal();
			return err("Could not upload texture to gpu");
		}

		if (!keepCPUCopy) {
			newTexture.FreeLocal();
		}

		result = textures.size();
		textures.push_back(newTexture);

		textureIDs.insert(std::make_pair(filePath, result));

		return ok(result);
	}

	Result<TextureID> TextureSystem::IsTextureLoaded(const std::string &filePath) {
		auto numElements = textureIDs.count(filePath);

		if (numElements > 0) {
			auto result = textureIDs.at(filePath);
			return ok(result);
		} else {
			return err("Texture doesn't exist");
		}
	}

	Result<Texture *> TextureSystem::GetTexture(TextureID id) {
		if (id < textures.size()) {
			return ok(&textures.at(id));
		}
		return err("no texture with this id was found!");
	}
}