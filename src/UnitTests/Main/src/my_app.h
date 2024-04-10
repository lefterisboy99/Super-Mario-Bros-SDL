#pragma once
#include <app.h>
#include <gridmap.h>
#include <layer.h>
#include <spritesheet.h>
#include <sprite_system.h>
using namespace app;


class MyApp : public app::App {
public:
	MyApp(Game *myGame);
	void Initialize(void) override;
	void Load(void) override;
	void Clear(void) override;
};

void my_Render(Game& game);
void my_Anim(Game& game);
void my_Input(Game& game);
void my_AI(Game& game);
void my_Physics(Game& game);
void my_Destruct(Game& game);
void my_Collisions(Game& game);
void my_User(Game& game);
void my_On_Quit(app::Event &event);

engine::GridMap *ExportGridMap(const engine::Tilemap *tilemap, int tile_subdivisions);
engine::GridMap *ExportGridMap2(const engine::Tilemap *tilemap, const engine::Texture &texture, int tile_subdivisions);
