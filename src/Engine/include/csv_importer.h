#pragma once

#include <string>
#include <vector>

namespace engine {
	class CSVImporter {
	public:
		using FileData = std::vector<std::vector<std::string>>;
		using Row = std::vector<std::string>;
		class File {
		public:
			File(){}

			size_t NumRows() const { return numRows; }
			size_t MaxColumns() const { return maxColumns; }
			friend class CSVImporter;

			const Row At(size_t i) const;
			size_t maxColumns, numRows;

			std::vector<size_t>* ParseUInt(size_t &outRows);

			const std::string& Path() const { return path; }
		private:
			std::string path;
			FileData data;
		};
		static File Load(std::string path);
	};

}