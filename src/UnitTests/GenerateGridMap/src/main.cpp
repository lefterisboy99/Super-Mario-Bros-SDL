#include <my_app.h>
#include <fstream>

static engine::GridMap *Generate_Grid_Map(MyGame game, char *tilemap_csv, char *tileset, int tile_subdivisions, int tile_size) {

	game.tilemapTexture = game.LoadTexture(tileset).unwrap();
	engine::TilemapSystem &ts = game.TilemapSystem();

	static engine::CSVImporter::File file = engine::CSVImporter::Load(tilemap_csv);
	game.levelTilemap = ts.CreateTilemapFromCSV(file, tile_size).unwrap();

	return ExportGridMap(game.TilemapSystem().Map().Get(game.levelTilemap), tile_subdivisions);

}

static void Export_Grid_Map_To_CSV(engine::GridMap *gridMap, const std::string &path = "-") {

	std::ofstream file;

	if(path == "-") file = std::ofstream("./media/Grid_Map.csv");
	else  file = std::ofstream(path.c_str());

	gridMap->ForEachRow([&](size_t row, size_t col, const engine::GridTile &tile){
		file << (tile == engine::GridTile::Empty ? 0 : 1);
		file << (col == (gridMap->NumLogicalColumns() * gridMap->NumSubtiles()) - 1 ? "\n" : ",");
	});

	file.close();
	return;
}

int main(int argc, char *argv[]) {
	if (argc < 5) {
		std::cout << "Wrong arguments\n";
		std::cout << "The correct format is:\n\n\t <executable_name> <tilemap.csv> <tileset.png> <tile_subdivisions> <tile_size>\n";
		return EXIT_FAILURE;
	}

	MyGame game;
	MyApp app(&game);

	game.collisionMap = Generate_Grid_Map(game, argv[1], argv[2], atoi(argv[3]), atoi(argv[4]));

	Export_Grid_Map_To_CSV(game.collisionMap, "./media/Grid_Map.csv");

	return 0;
}