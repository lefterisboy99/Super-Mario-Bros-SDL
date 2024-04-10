#include <gridmap.h>
#include <SDL.h>
#include <common.h>
#include <string.h>

namespace engine {

GridMap::GridMap(int numRows, int numColumns, int tileSize, int subtileCount) {
	ASSERT(subtileCount > 0);

	elements = new GridTile[numRows * numColumns * subtileCount * subtileCount];
	ASSERT(elements);
	memset(elements, (int)GridTile::Empty, numRows * numColumns * subtileCount * subtileCount * sizeof(GridTile));

	this->numLogicalRows = numRows;
	this->numLogicalColumns = numColumns;
	this->subtileCount = subtileCount;
	this->tileSize = tileSize;
}

GridTile GridMap::Get(int row, int col, int subRow, int subCol) {
	int numRows = numLogicalRows * subtileCount;
	int numCols = numLogicalColumns * subtileCount;

	if (subRow >= subtileCount) return GridTile::Solid;
	if (subCol >= subtileCount) return GridTile::Solid;
	if (row >= numLogicalRows) return GridTile::Solid;
	if (col >= numLogicalColumns) return GridTile::Solid;

	int actualRow = row * subtileCount + subRow;
	int actualCol = col * subtileCount + subCol;

	if (actualRow < 0 || actualCol < 0) return GridTile::Solid;

	// return elements[(row * numColumns + col) * subtileCount + subRow * subtileCount + subCol];
	return elements[IndexOf(actualRow, actualCol)];
}

void GridMap::Set(int row, int col, int subRow, int subCol, GridTile tile) {
	ASSERT((subRow < subtileCount) && (subCol < subtileCount));

	int numRows = numLogicalRows * subtileCount;
	int numCols = numLogicalColumns * subtileCount;

	int actualRow = row * subtileCount + subRow;
	int actualCol = col * subtileCount + subCol;
	elements[actualRow * numCols + actualCol] = tile;
}

void GridMap::SetAll(int row, int col, GridTile tile) {
	ASSERT(row < NumLogicalRows());
	ASSERT(col < NumLogicalColumns());

	int numRows = numLogicalRows * subtileCount;
	int numCols = numLogicalColumns * subtileCount;

	for (int subCol = 0ul; subCol < subtileCount; ++subCol) {
		for (int subRow = 0ul; subRow < subtileCount; ++subRow) {

			int actualRow = row * subtileCount + subRow;
			int actualCol = col * subtileCount + subCol;

			elements[IndexOf(actualRow, actualCol)] = tile;
		}
	}
}

void GridMap::ForEach(std::function<void(int,int,const GridTile&)> cb) const {
	int numRows = numLogicalRows * subtileCount;
	int numCols = numLogicalColumns * subtileCount;

	for (int subCol = 0ul; subCol < numCols; ++subCol) {
		for (int subRow = 0ul; subRow < numRows; ++subRow) {
			// SDL_Log("r, c = %u, %u", subRow, subCol);
			cb(subRow, subCol, elements[IndexOf(subRow, subCol)]);
		}
	}
}

void GridMap::ForEachRow(std::function<void(int,int,const GridTile&)> cb) const {
	int numRows = numLogicalRows * subtileCount;
	int numCols = numLogicalColumns * subtileCount;

	for (int subRow = 0ul; subRow < numRows; ++subRow) {
		for (int subCol = 0ul; subCol < numCols; ++subCol) {
			cb(subRow, subCol, elements[IndexOf(subRow, subCol)]);
		}
	}
}



}