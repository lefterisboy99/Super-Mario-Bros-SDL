#pragma once
#include <SDL.h>
#include <functional>

namespace engine {
	enum class GridTile {
		Solid,
		Empty
	};

	class GridMap {
	public:
		GridMap(int numRows, int numColumns, int tileSize, int subtileCount = 1);
		~GridMap() {
			delete[] elements;
		}

		GridTile Get(int row, int col, int subRow, int subCol);
		void Set(int row, int col, int subRow, int subCol, GridTile tile);
		void SetAll(int row, int col, GridTile tile);

		int GetPositionY_Down(int row, int subRow) {
			// TODO: hack right here on subRow - 1
			return row * tileSize + (subRow) * (tileSize / subtileCount);
		}

		int GetPositionY_Up(int row, int subRow) {
			// TODO: hack right here on subRow - 1
			return row * tileSize + (subRow) * (tileSize / subtileCount) + tileSize / subtileCount;
		}

		int GetPositionX_Right(int col, int subCol) {

			// return col * tileSize + (subCol) * (tileSize / subtileCount) - tileSize / subtileCount;
			return col * tileSize + (subCol) * (tileSize / subtileCount);
		}

		int GetPositionX_Left(int col, int subCol) {

			return col * tileSize + (subCol) * (tileSize / subtileCount) + tileSize / subtileCount;
		}

		void ForEach(std::function<void(int,int,const GridTile&)> cb) const;
		void ForEachRow(std::function<void(int,int,const GridTile&)> cb) const;

		int IndexOf(int subRow, int subCol) const {
			return subRow * (numLogicalColumns * subtileCount) + subCol;
		}

		int NumLogicalRows() const { return numLogicalRows;}
		int NumLogicalColumns() const { return numLogicalColumns;}
		int NumSubtiles() const { return subtileCount; }
		int TileSize() const { return tileSize; }

		inline int Width() const { return tileSize*numLogicalColumns; }
		inline int Height() const { return tileSize*numLogicalRows; }

	private:
		GridTile *elements;
		int numLogicalRows, numLogicalColumns, subtileCount;
		int tileSize;
	};
}