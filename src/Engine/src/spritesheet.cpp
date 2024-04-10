#include <spritesheet.h>

namespace engine {

	void Spritesheet::Tiles(TextureSystem& textureSystem, TextureID id, int tileSize) {
		parts.clear();

		SpritesheetPart part;
		Texture *texture = textureSystem.GetTexture(id).unwrap();
		int numCols = texture->Width() / tileSize;
		int numRows = texture->Height() / tileSize;

		for (int row = 0; row < numRows; ++row) {
			for (int col = 0; col < numCols; ++col) {
				Recti r(col * tileSize, row * tileSize, tileSize, tileSize);
				part.Add(r);
			}
		}

		Add(part);
	}

}