#include "my_app.h"
#include <iostream>
#include <csv_importer.h>
#include <error.h>
#include <utility>
#include <color.h>
#include <fmt/core.h>
#include <math2d.h>
#include "animations.h"

// Level-Wide globals =========================================================
engine::Spritesheet spritesheet;
engine::Spritesheet spritesheet1;
engine::Spritesheet spritesheet2;
engine::Spritesheet mushroomAnimationSet;
engine::TextureID tilemapTextureID;
engine::TextureID marioTexture;
// ============================================================================

// Mario sprite Globals =======================================================
float maxTimeForJump = 0.35f;
float currentTimeForJump = 0.0f;
bool canJump = false;
// ============================================================================

bool myOnSolidGround(const Recti &r);

void mario_on_start_falling() {

}

void mario_on_stop_falling() {

}

// Levels

struct Level {
	engine::Sprite *mario;
	engine::Layer layer;
	engine::GridMap *collisionMap;

	std::string name;
	std::function<void(const std::string, Game &game)> requestLevelSwitchHandler;

	virtual void Setup(Game &game) {}
	virtual void Cleanup(Game &game) {}
	virtual void Start(Game &game) {}
	virtual void AI(Game &game) {}
	virtual void Collisions(Game &game) {}

	virtual void SwitchFrom(Game &game, const std::string &other) {}

	virtual engine::TilemapID GetMainTilemap() { return 0; }
};

int current_border = 0;

#include "level1.cpp"
#include "level1.pipe.cpp"

Level *LEVEL_MAIN, *LEVEL_MAIN_PIPE;
Level *Current_Level = 0;

void LevelSwitch(const std::string levelName, Game &game) {

	Current_Level->Cleanup(game);

	const std::string &prevName = Current_Level->name;

	if (levelName == "level1.pipe") {
		fmt::print("level 1 pipe");
		Current_Level = LEVEL_MAIN_PIPE;
	} else {
		fmt::print("level 1");
		Current_Level = LEVEL_MAIN;
	}

	Current_Level->Start(game);
	Current_Level->SwitchFrom(game, prevName);
}

inline engine::GridMap *GetCurrentCollisionMap() {
	return Current_Level->collisionMap;
}

inline engine::Layer *GetCurrentLayer() {
	return &Current_Level->layer;
}

inline engine::Sprite *GetMario() {
	return Current_Level->mario;
}

inline engine::TilemapID GetCurrentTilemap() {
	return Current_Level->GetMainTilemap();
}

// Player
// --------------------------------
// engine::Sprite *sprite;
// engine::Sprite *goomba[2];
// engine::Sprite *coin[2];


// --------------------------------

bool myOnSolidGround(const Recti &r) {
	int dummy;
	return !(FilterGridDown(GetCurrentCollisionMap(), r.x, r.y, r.w, r.h, dummy));
}



using namespace app;

void my_Render(Game& myGame) {

	const engine::Texture *tilemapTexture = myGame.GetTexture(tilemapTextureID).unwrap();
	engine::RenditionSystem &rend = myGame.GetRenderer();

	uint32_t imageWidth = tilemapTexture->Width();
	uint32_t imageHeight = tilemapTexture->Height();

	GetCurrentLayer()->ForEach([&](int row, int col, int tile) {
		const engine::Tilemap *current_tilemap = GetCurrentLayer()->GetCurrentItem();

		int x = int(col * current_tilemap->tileSize);
		int y = int(row * current_tilemap->tileSize);

		SDL_Rect crop = current_tilemap->crop(tile, imageWidth);
		rend.RenderTexture(tilemapTexture->Handle(), x, y, current_tilemap->tileSize, current_tilemap->tileSize, &crop);
	});

	engine::Sprite *sprite = GetMario();

	// if (collisionMap) {
	// 	int subtiles = (int)collisionMap->NumSubtiles();
	// 	int subtilePixelSize = int(tilemap->tileSize / subtiles);

	// 	collisionMap->ForEach([&](int subRow, int subCol, const engine::GridTile& solidity){
	// 		int x = int(subCol * subtilePixelSize);
	// 		int y = int(subRow * subtilePixelSize);

	// 		if (solidity == engine::GridTile::Solid)
	// 			rend.RenderRect(x, y, subtilePixelSize, subtilePixelSize);
	// // 		// else
	// // 			// rend.RenderRect(x + (int)myGame.scrollX, y + (int)myGame.scrollY, subtilePixelSize, subtilePixelSize, engine::Color(0,255,0,255));

	// 	});
	// }
	// rend.FillRect(float(myGame.character_posx), float(myGame.character_posy), (float)myGame.height, (float)myGame.width);

	rend.Text(fmt::format("Velocity: [{}, {}], falling: {} {}, jumping: {} ", sprite->velocity.x, sprite->velocity.y, currentTimeForJump, maxTimeForJump, canJump), {0,0});

	auto playerBox = sprite->GetBox();
	auto viewBox = rend.GetViewBox();

	rend.RenderRect(viewBox.x, viewBox.y, viewBox.w, viewBox.h, engine::Color::Red());
	rend.RenderRect(playerBox.x, playerBox.y, playerBox.w, playerBox.h, engine::Color::Green());

	rend.Text(fmt::format("Player in View: {}", viewBox.Contains(playerBox)), {0,32});

	for (auto *sprite : myGame.SpriteSystem().GetDisplayList()) {
		sprite->MoveAnimationTime(myGame.DeltaTime());
		sprite->Display(rend);
	}

}

void my_Anim(Game& game) {

	engine::Sprite *sprite = GetMario();
	engine::GridMap *collisionMap = GetCurrentCollisionMap();

	// MARIO
	if(collisionMap) {
		if (sprite->velocity.x != 0 && myOnSolidGround(sprite->GetBox())) {
			int index;
			const TiledAnimation *moveAnimation = GetAnimation(
				MARIO_SMALL_ANIMATIONS, ARRAY_COUNT(MARIO_SMALL_ANIMATIONS),
				"small_move",
				index).unwrap();

			float animTime = moveAnimation->totalTime;

			sprite->SetAnimation(index, 0.0f, animTime);

			sprite->scale.x = sgn(sprite->velocity.x);
		} else if (!myOnSolidGround(sprite->GetBox())) {

			int index;
			const TiledAnimation *jumpAnimation = GetAnimation(
				MARIO_SMALL_ANIMATIONS, ARRAY_COUNT(MARIO_SMALL_ANIMATIONS),
				"small_jump",
				index).unwrap();

			float animTime = jumpAnimation->totalTime;

			sprite->SetAnimation(index, 0.0f, animTime);

		} else {
			sprite->SetAnimation(0,0,0);
		}


	}
}

void marioInputLogic(Game &myGame, int moveByX, int moveByY) {
	engine::Sprite *sprite = GetMario();

	// Mario input logic
	// ========================================================================
	int yMove = 0;
	if (moveByY < 0 && canJump) {

		if (currentTimeForJump >= maxTimeForJump) {
			yMove = 1;
			canJump = false;
		} else {
			// are we currently jumping?
			if (sprite->velocity.y < 0) {
				currentTimeForJump += myGame.DeltaTime();
			} else {
				// started jumping this frame

				// if (!sprite->GetGravityHandler().IsFalling())
				currentTimeForJump = 0.0f;
			}

			if (currentTimeForJump < maxTimeForJump) {
				canJump = true;
				yMove = moveByY * 4;
			}
		}
	} else {
		if (sprite->GetGravityHandler().IsFalling()) {
			yMove = 1;
			canJump = false;
		} else {
			currentTimeForJump = 0.0f;
			yMove = 0;
			canJump = true;
		}
	}

	Vec2i by = (Vec2i{int(moveByX) *  1, yMove});
	if (by.x == 0) by.x = -sprite->velocity.x;
	sprite->Accelerate(by, GetCurrentCollisionMap());
	// ========================================================================
}


void my_Input(Game& myGame) {
	// std::cout << "DefaultView.offset.x: " << myGame.GetRenderer().DefaultView.offset.x << ", current_border: " << current_border << "Player x: " << sprite->position.x << "\n";
	engine::Sprite *sprite = GetMario();

	int moveByX = (myGame.GetKeyDown(SDL_SCANCODE_D) - myGame.GetKeyDown(SDL_SCANCODE_A));
	int moveByY = (myGame.GetKeyDown(SDL_SCANCODE_S) - myGame.GetKeyDown(SDL_SCANCODE_W));

	int scrollMapX = (myGame.GetKeyDown(SDL_SCANCODE_LEFT) - myGame.GetKeyDown(SDL_SCANCODE_RIGHT));
	auto &rendition = myGame.GetRenderer();

	int sprspeed = 1; // NOTE: cannot apply delta time

	// NOTE: 640 is the width of the actual window of the application
	if(sprite->position.x > current_border + ((640 / myGame.GetRenderer().ZoomLevel) / 2) - sprite->size.x ) {

		rendition.DefaultView.offset.x += abs(sprite->velocity.x);
		current_border += abs(sprite->velocity.x);
	}


	float moveByXMouse, moveByYMouse;
	myGame.GetMouseDelta(moveByXMouse, moveByYMouse);

	const float speed = 1000.0f;
	const float speedMouse = 55.f;

	if (myGame.GetButtonDown(engine::ButtonType::Left)) {
		myGame.GetRenderer().DefaultView.offset.x += int(moveByX * speed * myGame.DeltaTime());
		myGame.GetRenderer().DefaultView.offset.y += int(moveByY * speed * myGame.DeltaTime());

		myGame.GetRenderer().DefaultView.offset.x += int(moveByXMouse * speedMouse * myGame.DeltaTime());
		myGame.GetRenderer().DefaultView.offset.y += int(moveByYMouse * speedMouse * myGame.DeltaTime());
	}

	marioInputLogic(myGame, moveByX, moveByY);
}

void my_AI(Game& game) {
	// Goomba0

	// TODO: level ai
	Current_Level->AI(game);
}

void my_Physics(Game& game) {

	engine::Sprite *sprite = GetMario();
	engine::GridMap *collisionMap = GetCurrentCollisionMap();

	// MARIO
	if(sprite->position.x < current_border) sprite->position.x = current_border;
	if(collisionMap) {
		if (sprite->position.x > collisionMap->Width()) sprite->position.x = collisionMap->Width() - sprite->size.x;
		if (sprite->position.y < 0) sprite->position.y = 0;
		if (sprite->position.y > collisionMap->Height()) sprite->position.y = collisionMap->Height() - sprite->size.y;
	}

	auto &mushroomList = game.SpriteSystem().GetTypeList("mushroom");
	for (auto *mushroom : mushroomList) {
		int yMove = 0;
		if (mushroom->GetGravityHandler().IsFalling()) {
			yMove = 1;
		}
		mushroom->Move(Vec2i{1, yMove}, collisionMap);
	}


}

void my_Destruct(Game& game) {

}

void my_Collisions(Game& game) {

	Current_Level->Collisions(game);
	engine::GridMap *collisionMap = GetCurrentCollisionMap();



}

void my_User(Game& game) {

}

void my_On_Quit(app::Event &event) {

}

MyApp::MyApp(Game *myGame) :app::App(static_cast<Game*>(myGame)) {

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



	GetDispatcher().Register("Key", [=](app::App& app, const Event &event) {
		const engine::KeyEvent &keyEvent = dynamic_cast<const engine::KeyEvent&>(event);

		if (keyEvent.scancode == SDL_SCANCODE_HOME && !keyEvent.pressed) {
			game->GetRenderer().DefaultView.offset.x = 0;
			game->GetRenderer().DefaultView.offset.y = 0;
		} else if (keyEvent.scancode == SDL_SCANCODE_END && !keyEvent.pressed) {
			const int width = 640;
			const int height = 480;

			const engine::Tilemap *tilemap = game->TilemapSystem().Map().Get(GetCurrentTilemap());

			const int tilemapWidth = tilemap->NumColumns() * tilemap->tileSize;
			const int tilemapHeight = tilemap->NumRows() * tilemap->tileSize;

			game->GetRenderer().DefaultView.offset.x = width - tilemapWidth;
			game->GetRenderer().DefaultView.offset.y = height - tilemapHeight;
		}

		if (keyEvent.scancode == SDL_SCANCODE_UP && !keyEvent.pressed) {
			game->GetRenderer().ZoomLevel += 0.25f;
		} else if (keyEvent.scancode == SDL_SCANCODE_DOWN && !keyEvent.pressed) {
			game->GetRenderer().ZoomLevel -= 0.25f;
		}

		// if (keyEvent.scancode == SDL_SCANCODE_W && !keyEvent.pressed) {
		// 	sprite->Impulse(Vec2i{0, -5}, collisionMap);
		// }


	});
}

// void UpdateGridmap(Game *game) {

// 	const engine::Tilemap *tilemap = game->TilemapSystem().Map().Get(GetCurrentTilemap());
// 	const engine::Texture *tilemapTexture = game->GetTexture(tilemapTextureID).unwrap();

// 	if (collisionMap) delete collisionMap;
// 	//game->collisionMap = ExportGridMap2(tilemap, *tilemapTexture, 2);
// 	collisionMap = ExportGridMap(tilemap, 2);

// }


void MyApp::Load(void) {
	std::cout << "load\n";

	tilemapTextureID = game->LoadTexture("media/super_mario_sprite_sheet.png", true).unwrap();
	marioTexture = game->LoadTexture("media/super_mario.png").unwrap();


	game->GetRenderer().ZoomLevel = 1.0f;

	// mario

	engine::Texture *marioTextureInstance = game->GetTexture(marioTexture).unwrap();
	BuildSpritesheet(marioTextureInstance, spritesheet, MARIO_SMALL_ANIMATIONS, ARRAY_COUNT(MARIO_SMALL_ANIMATIONS), {32, 32});

	engine::Texture *gommbaTextureInstance = game->GetTexture(tilemapTextureID).unwrap();
	BuildSpritesheet(gommbaTextureInstance, spritesheet1, GOOMBA_ANIMATIONS, ARRAY_COUNT(GOOMBA_ANIMATIONS), {16, 16});

	BuildSpritesheet(gommbaTextureInstance, spritesheet2, COIN_ANIMATIONS, ARRAY_COUNT(COIN_ANIMATIONS), {16, 16});

	BuildSpritesheet(gommbaTextureInstance, mushroomAnimationSet, MUSHROOM_ANIMATIONS, ARRAY_COUNT(MUSHROOM_ANIMATIONS), {16, 16});
	// for (int i = 0; i < 14; ++i) {
	// 	Recti r = {
	// 		32 * i, 96, 32, 32
	// 	};
	// 	mainPart.Add(r);

	// }

	LEVEL_MAIN = new LevelMain();
	LEVEL_MAIN_PIPE = new LevelPipe();

	LEVEL_MAIN->Setup(*game);
	LEVEL_MAIN_PIPE->Setup(*game);

	LEVEL_MAIN->requestLevelSwitchHandler = LevelSwitch;
	LEVEL_MAIN_PIPE->requestLevelSwitchHandler = LevelSwitch;

	Current_Level = LEVEL_MAIN;
	Current_Level->Start(*game);

	size_t mainMusicIndex = game->AudioSystem().LoadMusic("media/main_theme.mp3");
	game->AudioSystem().SetVolume(0);
	game->AudioSystem().PlayMusic(mainMusicIndex, true);

	// UpdateGridmap(game);

}

void MyApp::Clear(void) {
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

	engine::GridMap *newGridMap = new engine::GridMap((int)tilemap->NumRows(), (int)tilemap->NumColumns(), tilemap->tileSize, tile_subdivisions);
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

	int tileSize = tilemap->tileSize;
	engine::GridMap *newGridMap = new engine::GridMap((int)tilemap->NumRows(), (int)tilemap->NumColumns(), tileSize, tile_subdivisions);
	tilemap->ForEach([&](int row, int col, int tileIndex) {
		newGridMap->SetAll(row, col, tileIndex == -1
			? engine::GridTile::Empty
			: engine::GridTile::Solid);
	});

	return newGridMap;
}