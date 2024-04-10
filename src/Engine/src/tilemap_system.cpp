#include <tilemap_system.h>
#include <SDL.h>

namespace engine {


Result<TilemapID> TilemapSystem::CreateTilemapFromCSV(const CSVImporter::File &loadedFile, int tileSize) {
	Tilemap *result = new Tilemap();
	if (!result)
		return err("Could not allocate memory to store tilemap");

	result->tileSize = tileSize;
	result->numRows = (int)loadedFile.NumRows();
	result->numColumns = (int)loadedFile.MaxColumns();
	result->data = new int[result->numRows * result->numColumns];
	if (!result->data) {
		return err("Could not allocate memory to store tilemap data");
	}

	for (int row = 0u; row < result->numRows; ++row) {
		auto fileRow = loadedFile.At(row);
		for (int column = 0u; column < result->numColumns; ++column) {
			auto str = fileRow.at(column);
			result->data[row * result->numColumns + column] = std::stol(str);
		}
	}

	auto id = tilemaps.Add(result, loadedFile.Path());
	return ok(id);
}

TilemapSystem::~TilemapSystem() {
	std::cout << "~TilemapSystem" << std::endl;
	tilemaps.Delete();
}

void Tilemap::ChangeTile(int row, int col, int tile_index) {
	data[row * numColumns + col] = tile_index;
}

void Tilemap::ForEach(std::function<void(int,int,int)> cb) const {
	for (int row = 0; row < numRows; ++row) {
		for (int column = 0; column < numColumns; ++column) {
			cb(row, column, At(row, column));
		}
	}
}


}