#include <csv_importer.h>
#include <sstream>
#include <iostream>
#include <fstream>

namespace engine {
	using Row = std::vector<std::string>;
	CSVImporter::File CSVImporter::Load(std::string path) {
		CSVImporter::File result;

		std::ifstream f(path);

		// TODO: specific filesystem exception
		if (!f.good()) {
			throw new std::exception("Failed to open file");
		}

		FileData rowsColumns;

		std::string currentLine;

		result.numRows = 0;
		result.maxColumns = 0u;
		while (std::getline(f, currentLine)) {
			std::stringstream linestream(currentLine);
			std::vector<std::string> row;
			std::string item;

			size_t columns = 0;
			while (std::getline(linestream, item, ',')) {
				row.push_back(item);
				columns++;
			}

			if (columns > result.maxColumns) result.maxColumns = columns;

			rowsColumns.push_back(row);
			result.numRows++;
		}
		result.data = std::move(rowsColumns);
		result.path = path;

		return result;
	}


	std::vector<size_t>* CSVImporter::File::ParseUInt(size_t &outRows) {

		std::vector<size_t>* result = new std::vector<size_t>[NumRows()];
		if (!result) return nullptr;

		size_t cRow = 0u;
		for (auto row : data) {
			std::vector<size_t> &outRow = result[cRow];

			size_t cCol = 0u;
			for (auto item : row) {

				outRow.push_back(std::stol(item));
			}

			cRow++;
		}

		outRows = cRow;
		return result;
	}

	const Row CSVImporter::File::At(size_t i) const {
		return data.at(i);
	}
}