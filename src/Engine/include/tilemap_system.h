#pragma once
#include <common.h>
#include <csv_importer.h>
#include <iostream>
#include <functional>
#include <error.h>
#include <SDL.h>
#include <indirection_map.h>
#include <math2d.h>

namespace engine {

typedef uint64_t TilemapID;

class TilemapSystem;
class Tilemap {
public:
	int At(int row, int column) const {
		if (row < numRows && column < numColumns) {
			return data[row * numColumns + column];
		} else {
			return ~(0U);
		}
	}

	inline Vec2i Pixel(int row, int column) {
		return {
			column * tileSize,
			row * tileSize
		};
	}

	inline SDL_Rect crop(int tile, int width) const {
		int blocks = width / (int)tileSize;

		return {
			(int)(tile % blocks) * (int)tileSize,
			(int)(tile / blocks) * (int)tileSize,
			(int)tileSize,
			(int)tileSize
		};
	}

	void ChangeTile(int row, int col, int tile_index);

	void ForEach(std::function<void(int,int,int)> cb) const;
	int tileSize;

	int NumRows() const { return numRows; }
	int NumColumns() const { return numColumns; }
	~Tilemap() {
		delete[] data;
	}

private:
	Tilemap() {}

	int *data;
	int numRows;
	int numColumns;

	friend class TilemapSystem;
};

class TilemapSystem {
public:
	Result<uint64_t> CreateTilemapFromCSV(const CSVImporter::File &loadedFile, int tileSize);

	const IndirectionMap<Tilemap*>& Map() const {return tilemaps; }
	~TilemapSystem();
private:
	IndirectionMap<Tilemap*> tilemaps;
};


}