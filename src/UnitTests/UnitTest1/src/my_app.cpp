#include "my_app.h"
#include <iostream>
#include <csv_importer.h>
#include <error.h>
#include <utility>
#include <color.h>
#include <fmt/core.h>
#include <math2d.h>

using namespace app;

void my_Render(Game& game) {

	MyGame &myGame = (MyGame&)game;
	const engine::Texture *tilemapTexture = myGame.GetTexture(myGame.tilemapTexture).unwrap();
	const engine::Tilemap *tilemap = myGame.TilemapSystem().Map().Get(myGame.levelTilemap);

	uint32_t imageWidth = tilemapTexture->Width();
	uint32_t imageHeight = tilemapTexture->Height();

	engine::RenditionSystem &rend = myGame.GetRenderer();

	myGame.level1Layer.ForEach([&](int row, int col, int tile) {
		const engine::Tilemap *current_tilemap = myGame.level1Layer.GetCurrentItem();

		int x = int(col * current_tilemap->tileSize);
		int y = int(row * current_tilemap->tileSize);

		SDL_Rect crop = current_tilemap->crop(tile, imageWidth);
		rend.RenderTexture(tilemapTexture->Handle(), x, y, current_tilemap->tileSize, current_tilemap->tileSize, &crop);
	});

	if (myGame.collisionMap) {
		int subtiles = (int)myGame.collisionMap->NumSubtiles();
		int subtilePixelSize = int(tilemap->tileSize / subtiles);

		myGame.collisionMap->ForEach([&](int subRow, int subCol, const engine::GridTile& solidity){
			int x = int(subCol * subtilePixelSize);
			int y = int(subRow * subtilePixelSize);

			if (solidity == engine::GridTile::Solid)
				rend.RenderRect(x, y, subtilePixelSize, subtilePixelSize);
			// else
				// rend.RenderRect(x + (int)myGame.scrollX, y + (int)myGame.scrollY, subtilePixelSize, subtilePixelSize, engine::Color(0,255,0,255));

		});
	}
	// rend.FillRect(float(myGame.character_posx), float(myGame.character_posy), (float)myGame.height, (float)myGame.width);

	myGame.sprite->MoveAnimationTime(myGame.DeltaTime());
	myGame.sprite->Display(rend);

	// rend.Text(fmt::format("Player X: {}, Player Y: {}", myGame.character_posx, myGame.character_posy), {0,0});
}

void my_Anim(Game& game) {

}


void Calculate_position_in_element(int x,int y,int *ac_r,int *ac_c,int *subrow,int *subcol){
		*ac_r=y/8;
		*ac_c=x/8;
		*subrow=*ac_r%2;
		*subcol=*ac_c%2;

}


bool FilterGridLeft(engine::GridMap *gridmap,int x,int y,int wide){
	int ac_r,ac_c,subrow,subcol;
	for(int j=y;j<y+wide;j++){
			Calculate_position_in_element(x-1, j,&ac_r,&ac_c,&subrow,&subcol);

			if(gridmap->Get(ac_r/2,ac_c/2,ac_r%2,ac_c%2)== engine::GridTile::Solid){
				std::cout<<"Solid Left"<<std::endl;
				return 0;
			}
		}
	return 1;
}

bool FilterGridRight(engine::GridMap *gridmap,int x,int y,int height,int wide){
	int ac_r,ac_c,subrow,subcol;
	for(int j=y;j<y+wide;j++){
			Calculate_position_in_element(x+height+1, j,&ac_r,&ac_c,&subrow,&subcol);

			if(gridmap->Get(ac_r/2,ac_c/2,ac_r%2,ac_c%2)== engine::GridTile::Solid){
				std::cout<<"Solid Right"<<std::endl;
				return 0;
			}
		}
	return 1;
}

bool FilterGridUp(engine::GridMap *gridmap,int x,int y,int height){
	int ac_r,ac_c,subrow,subcol;
	for(int i=x;i<x+height;i++){
			Calculate_position_in_element(i, y-1,&ac_r,&ac_c,&subrow,&subcol);

			if(gridmap->Get(ac_r/2,ac_c/2,ac_r%2,ac_c%2)== engine::GridTile::Solid){
				std::cout<<"Solid Up"<<std::endl;
				return 0;
			}
		}
		return 1;
}

bool FilterGridDown(engine::GridMap *gridmap,int x,int y,int height,int wide){
	int ac_r,ac_c,subrow,subcol;
	for(int i=x;i<x+height;i++){
			Calculate_position_in_element(i, y+wide+1,&ac_r,&ac_c,&subrow,&subcol);

			if(gridmap->Get(ac_r/2,ac_c/2,ac_r%2,ac_c%2)== engine::GridTile::Solid){
				std::cout<<"Solid Down"<<std::endl;
				return 0;
			}
		}
	return 1;
}



void my_Input(Game& game) {
	MyGame &myGame = static_cast<MyGame&>(game);

	float moveByX = ((float)game.GetKeyDown(SDL_SCANCODE_D) - (float)game.GetKeyDown(SDL_SCANCODE_A));
	float moveByY = ((float)game.GetKeyDown(SDL_SCANCODE_S) - (float)game.GetKeyDown(SDL_SCANCODE_W));

	float moveByXMouse, moveByYMouse;
	game.GetMouseDelta(moveByXMouse, moveByYMouse);

	const float speed = 1000.0f;
	const float speedMouse = 55.f;

	if (game.GetButtonDown(engine::ButtonType::Left)) {
		myGame.GetRenderer().DefaultView.offset.x += int(moveByX * speed * game.DeltaTime());
		myGame.GetRenderer().DefaultView.offset.y += int(moveByY * speed * game.DeltaTime());
		myGame.GetRenderer().DefaultView.offset.x += int(moveByXMouse * speedMouse * game.DeltaTime());
		myGame.GetRenderer().DefaultView.offset.y += int(moveByYMouse * speedMouse * game.DeltaTime());
	}

	int sprspeed = int(140 * myGame.DeltaTime());
	Vec2i by = (Vec2i{int(moveByX) *  sprspeed, int(moveByY) * sprspeed});
	fmt::print("x: {} y: {}\n", by.x, by.y);
	if (myGame.collisionMap) myGame.sprite->Move(by, myGame.collisionMap);

	// if(myGame.collisionMap){
	// 	if (!game.GetButtonDown(engine::ButtonType::Left)) {

	// 		if(moveByX>0){
	// 			if(FilterGridLeft(myGame.collisionMap,myGame.character_posx,myGame.character_posy,myGame.width))
	// 				myGame.character_posx-= int(100 * game.DeltaTime());
	// 		}
	// 		if(moveByX<0){
	// 			if(FilterGridRight(myGame.collisionMap,myGame.character_posx,myGame.character_posy,myGame.height,myGame.width))
	// 				myGame.character_posx+= int(100 * game.DeltaTime());
	// 		}
	// 		if(moveByY<0){
	// 			if(FilterGridDown(myGame.collisionMap,myGame.character_posx,myGame.character_posy,myGame.height,myGame.width))
	// 				myGame.character_posy+= int(100 * game.DeltaTime());
	// 		}
	// 		if(moveByY>0){
	// 			if(FilterGridUp(myGame.collisionMap,myGame.character_posx,myGame.character_posy,myGame.height))
	// 				myGame.character_posy-= int(100 * game.DeltaTime());
	// 		}

	// 	}
	// }


}

void my_AI(Game& game) {

}

void my_Physics(Game& game) {
	MyGame &myGame = static_cast<MyGame&>(game);

	if (myGame.collisionMap) {
		myGame.sprite->Move(Vec2i{0,int(100*game.DeltaTime())}, myGame.collisionMap);
	}
}

void my_Destruct(Game& game) {

}

void my_Collisions(Game& game) {

}

void my_User(Game& game) {

}

void my_On_Quit(app::Event &event) {

}


MyApp::MyApp(MyGame *myGame) :app::App(static_cast<Game*>(myGame)) {

	game->SetRender(my_Render);
	game->SetAnim(my_Anim);
	game->SetInput(my_Input);
	game->SetAI(my_AI);
	game->SetPhysics(my_Physics);
	game->SetDestruct(my_Destruct);
	game->SetCollisions(my_Collisions);
	game->SetUser(my_User);
}

void MyApp::Initialize(void) {
	std::cout << "init\n";

	MyGame *myGame = static_cast<MyGame *>(game);
	GetDispatcher().Register("Key", [=](app::App& app, const Event &event) {
		const engine::KeyEvent &keyEvent = dynamic_cast<const engine::KeyEvent&>(event);

		if (keyEvent.scancode == SDL_SCANCODE_HOME && !keyEvent.pressed) {
			myGame->GetRenderer().DefaultView.offset.x = 0;
			myGame->GetRenderer().DefaultView.offset.y = 0;
		} else if (keyEvent.scancode == SDL_SCANCODE_END && !keyEvent.pressed) {
			const int width = 640;
			const int height = 480;

			const engine::Tilemap *tilemap = myGame->TilemapSystem().Map().Get(myGame->levelTilemap);

			const int tilemapWidth = tilemap->NumColumns() * tilemap->tileSize;
			const int tilemapHeight = tilemap->NumRows() * tilemap->tileSize;

			myGame->GetRenderer().DefaultView.offset.x = width - tilemapWidth;
			myGame->GetRenderer().DefaultView.offset.y = height - tilemapHeight;
		}

		if (keyEvent.scancode == SDL_SCANCODE_UP && !keyEvent.pressed) {
			myGame->GetRenderer().ZoomLevel += 0.25f;
		} else if (keyEvent.scancode == SDL_SCANCODE_DOWN && !keyEvent.pressed) {
			myGame->GetRenderer().ZoomLevel -= 0.25f;
		}


		// other stuff
		if (keyEvent.scancode == SDL_SCANCODE_SPACE && !keyEvent.pressed) {
			std::swap(myGame->tilemapTexture, myGame->tilemapTexture2);
			std::swap(myGame->levelTilemap, myGame->levelTilemap2);
		}

		if (keyEvent.scancode == SDL_SCANCODE_E && !keyEvent.pressed) {
			const engine::Tilemap *tilemap = myGame->TilemapSystem().Map().Get(myGame->levelTilemap);
			const engine::Texture *tilemapTexture = myGame->GetTexture(myGame->tilemapTexture).unwrap();

			if (myGame->collisionMap) delete myGame->collisionMap;
			myGame->collisionMap = ExportGridMap2(tilemap, *tilemapTexture, 2);
		}
	});
}

void MyApp::Load(void) {
	std::cout << "load\n";

	MyGame *myGame = static_cast<MyGame *>(game);

	myGame->tilemapTexture = myGame->LoadTexture("media/overworld_tileset_grass.png", true).unwrap();
	myGame->marioTexture = myGame->LoadTexture("media/super_mario.png").unwrap();
	myGame->tilemapTexture2 = myGame->LoadTexture("media/sewer_1.png").unwrap();


	// Tilemaps
	engine::TilemapSystem &ts = myGame->TilemapSystem();
	{
		engine::CSVImporter::File file = engine::CSVImporter::Load("media/map1_Kachelebene 1.csv");
		myGame->levelTilemap = ts.CreateTilemapFromCSV(file, 16).unwrap();
	}
	{
		engine::CSVImporter::File file = engine::CSVImporter::Load("media/map1_Tile Layer 2.csv");
		myGame->levelTilemap1 = ts.CreateTilemapFromCSV(file, 16).unwrap();
	}
	myGame->level1Layer = engine::Layer("Level1");
	myGame->level1Layer.PushNewTileMapOnLayer(myGame->TilemapSystem().Map().Get(myGame->levelTilemap));
	myGame->level1Layer.PushNewTileMapOnLayer(myGame->TilemapSystem().Map().Get(myGame->levelTilemap1));
	{
		engine::CSVImporter::File file = engine::CSVImporter::Load("media/map3.csv");
		myGame->levelTilemap2 = ts.CreateTilemapFromCSV(file, 16).unwrap();
	}

	// mario
	engine::SpritesheetPart mainPart;
	for (int i = 0; i < 14; ++i) {
		Recti r = {
			32 * i, 96, 32, 32
		};
		mainPart.Add(r);

	}
	myGame->spritesheet.Add(mainPart);

	myGame->sprite = new engine::Sprite(myGame->GetTextureSystem(), &myGame->spritesheet, myGame->marioTexture);
	myGame->sprite->SetAnimation(0, 0.f, 10.f);
	myGame->sprite->position = { 200, 400 };
	myGame->sprite->size = { 32,32 };
}

void MyApp::Clear(void) {
	std::cout << "clear\n";

}

#include <set>
using namespace engine;

struct Compare {
	bool operator()(const Color &a, const Color &b) const {
		return a.dword < b.dword;
	}
};

class ColorCache {
public:
	void Add(int idx, const Texture &texture, const Tilemap &tilemap) {
		if (indices.find(idx) == indices.end()) {
			indices.insert(idx);

			SDL_Rect r = tilemap.crop(idx, texture.Width());
			texture.Access(r, [this](Color c, uint32_t x, uint32_t y) {
				colors.insert(c);
			});
		}
	}

	bool Contains(const Color &c) const {
		return colors.find(c) != colors.end();
	}

	bool Empty() {
		return colors.size() == 0;
	}
private:
	std::set<int> indices;
	std::set<Color, Compare> colors;

};

engine::GridMap *ExportGridMap2(const engine::Tilemap *tilemap, const engine::Texture &texture, int tile_subdivisions) {

	engine::GridMap *newGridMap = new engine::GridMap((int)tilemap->NumRows(), (int)tilemap->NumColumns(), tile_subdivisions);
	ColorCache colorCache;
	tilemap->ForEach([&](int row, int col, int tileIndex) {

		bool tileAssumedEmpty = tileIndex <= 59;

		if (tileAssumedEmpty) {
			newGridMap->SetAll(row, col, engine::GridTile::Empty);

			colorCache.Add(tileIndex, texture, *tilemap);
		} else if (colorCache.Empty()) {
			newGridMap->SetAll(row, col, engine::GridTile::Solid);
		} else {

			for (int r = 0; r < tile_subdivisions; ++r) {
				for (int c = 0; c < tile_subdivisions; ++c) {
					SDL_Rect tileRect = tilemap->crop(tileIndex, texture.Width());

					int subtile = (int)tilemap->tileSize / tile_subdivisions;

					SDL_Rect checkRect = {
						tileRect.x + c*subtile,
						tileRect.y + r*subtile,
						subtile,
						subtile
					};

					uint32_t n = 0;
					uint32_t threshold = 12;
					texture.Access(checkRect, [&](Color c, uint32_t x, uint32_t y){
						if ((c.a != 0) && !colorCache.Contains(c)) {
							n++;
						}
					});

					newGridMap->Set(row, col, r, c, (n < threshold) ? GridTile::Solid : GridTile::Empty);
				}
			}
		}

	});

	return newGridMap;
}

engine::GridMap *ExportGridMap(const engine::Tilemap *tilemap, int tile_subdivisions) {

	engine::GridMap *newGridMap = new engine::GridMap((int)tilemap->NumRows(), (int)tilemap->NumColumns(), tile_subdivisions);
	tilemap->ForEach([&](int row, int col, int tileIndex) {
		newGridMap->SetAll(row, col, tileIndex != 61
			? engine::GridTile::Empty
			: engine::GridTile::Solid);
	});

	return newGridMap;
}