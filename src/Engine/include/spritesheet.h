#pragma once
#include <common.h>
#include <texture_system.h>
#include <math2d.h>
#include <error.h>
#include <vector>

namespace engine {

	class SpritesheetPart {
	public:
		inline void Add(const Recti &r) {rects.push_back(r); }
		Result<Recti*> At(size_t i) {
			if (i < rects.size()) return ok(&rects[i]);
			else return err("Index out of bounds.");
		}

		inline uint32_t Num() const { return (uint32_t)rects.size();}
	private:
		std::vector<Recti> rects;
	};

	class Spritesheet {
	public:
		inline void Add(const SpritesheetPart &part) {parts.push_back(part); }
		Result<SpritesheetPart*> At(size_t i) {
			if (i < parts.size()) return ok(&parts[i]);
			else return err("Index out of bounds.");
		}

		Result<Recti*> At(size_t i, size_t j) {
			if (i < parts.size()) return parts[i].At(j);
			else return err("Index out of bounds.");
		}

		uint32_t Num() const {
			return uint32_t(parts.size());
		}

		Result<uint32_t> Num(size_t i) const {
			if (i < parts.size()) return ok(parts[i].Num());
			else return err("Index out of bounds.");
		}

		void Tiles(TextureSystem& textureSystem, TextureID id, int tileSize);
	private:
		std::vector<SpritesheetPart> parts;
	};
}