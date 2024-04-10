
struct LevelMain : public Level {
	engine::Sprite *goomba[2];
	engine::Sprite *coin[2];

	engine::TilemapID terrain;
	engine::TilemapID background;
	engine::TilemapID sky;

	int goomba_xMove = 0;
	int goomba_xMove1 = 0;

	engine::TilemapID GetMainTilemap() override { return terrain; }

	void SetupMario(Game &game) {
		mario = new engine::Sprite(game.GetTextureSystem(), &spritesheet, marioTexture);
		mario->alive = true;
		// game->GetRenderer().DefaultView.offset.x = mario->position.x - ((640 / game->GetRenderer().ZoomLevel) / 2) - mario->size.x;
		// current_border = mario->position.x - ((640 / game->GetRenderer().ZoomLevel) / 2) - mario->size.x;

		// std::cout << "DefaultView.offset.x: " << game->GetRenderer().DefaultView.offset.x << ", current_border: " << current_border << "\n";

		auto& gravityHandler = mario->GetGravityHandler();
		gravityHandler.SetOnSolidGround(myOnSolidGround);
		gravityHandler.SetIsGravityAddicted(true);
		gravityHandler.SetOnStartFalling(mario_on_start_falling);
		gravityHandler.SetOnStopFalling(mario_on_stop_falling);
	}

	void SetupTilemaps(Game &game) {
		engine::TilemapSystem &ts = game.TilemapSystem();
		{
			engine::CSVImporter::File file = engine::CSVImporter::Load("media/lvl1_terrain.csv");
			terrain = ts.CreateTilemapFromCSV(file, 16).unwrap();
		}
		{
			engine::CSVImporter::File file = engine::CSVImporter::Load("media/lvl1_background.csv");
			background = ts.CreateTilemapFromCSV(file, 16).unwrap();
		}
		{
			engine::CSVImporter::File file = engine::CSVImporter::Load("media/lvl1_sky.csv");
			sky = ts.CreateTilemapFromCSV(file, 16).unwrap();
		}

		layer = engine::Layer("Level1");
		layer.PushNewTileMapOnLayer(game.TilemapSystem().Map().Get(sky));
		layer.PushNewTileMapOnLayer(game.TilemapSystem().Map().Get(background));
		layer.PushNewTileMapOnLayer(game.TilemapSystem().Map().Get(terrain));
	}

	void SetupCollision(Game &game) {
		const engine::Tilemap *tilemap = game.TilemapSystem().Map().Get(terrain);
		collisionMap = ExportGridMap(tilemap, 2);
	}

	void SetupEnemies(Game &game) {

		goomba[0] = new engine::Sprite(game.GetTextureSystem(), &spritesheet1, tilemapTextureID);
		goomba[1] = new engine::Sprite(game.GetTextureSystem(), &spritesheet1, tilemapTextureID);

		auto& gravityHandler1 = goomba[1]->GetGravityHandler();
		gravityHandler1.SetOnSolidGround(myOnSolidGround);
		gravityHandler1.SetIsGravityAddicted(true);

		coin[0] = new engine::Sprite(game.GetTextureSystem(), &spritesheet2, tilemapTextureID);
		coin[1] = new engine::Sprite(game.GetTextureSystem(), &spritesheet2, tilemapTextureID);
	}

	void Setup(Game &game) override {
		SetupTilemaps(game);
		SetupCollision(game);
		SetupMario(game);
		SetupEnemies(game);
		name = "level1";
	}

	void Cleanup(Game &game) override {
		game.SpriteSystem().Empty();
	}

	void Start(Game &game) override {

		game.GetRenderer().DefaultView.offset.x = 0;
		current_border = 0;

		mario->SetAnimation(0, 0.f, 10.f);
		mario->position = { 50, 50 };
		mario->size = { 16, 16 };
		mario->maxVelocity = { 2, 5};
		mario->minVelocity = { 2, 4};

		goomba[0]->type = "goomba";
		goomba[0]->SetAnimation(0, 0.f, 10.f);
		goomba[0]->position = { 80, 208 };
		goomba[0]->size = { 16, 16 };
		goomba[0]->maxVelocity = { 2, 5};
		goomba[0]->minVelocity = { 2, 4};

		goomba[1]->type = "goomba";
		goomba[1]->SetAnimation(0, 0.f, 10.f);
		goomba[1]->position = { 230, 208 };
		goomba[1]->size = { 16, 16 };
		goomba[1]->maxVelocity = { 2, 5};
		goomba[1]->minVelocity = { 2, 4};


		coin[0]->type = "coin";
		coin[0]->SetAnimation(0, 0.f, 10.f);
		coin[0]->position = { 280, 150 };
		coin[0]->size = { 16, 16 };
		coin[0]->maxVelocity = { 2, 5};
		coin[0]->minVelocity = { 2, 4};

		coin[1]->type = "coin";
		coin[1]->SetAnimation(0, 0.f, 10.f);
		coin[1]->position = { 300, 150 };
		coin[1]->size = { 16, 16 };
		coin[1]->maxVelocity = { 2, 5};
		coin[1]->minVelocity = { 2, 4};

		game.SpriteSystem().Add(mario);
		game.SpriteSystem().Add(goomba[0]);
		game.SpriteSystem().Add(goomba[1]);
		game.SpriteSystem().Add(coin[0]);
		game.SpriteSystem().Add(coin[1]);
	}

	void AI(Game &game) override {
		if(goomba[0]->position.x >= 140) {
			goomba_xMove = -1;
		} else if(goomba[0]->position.x <= 80) {
			goomba_xMove = 1;
		}
		if(collisionMap && goomba[0]->alive) goomba[0]->Move(Vec2i{goomba_xMove, 0}, collisionMap);

		// Goomba1
		if(goomba[1]->position.x >= 450) {
			goomba_xMove1 = -1;
		} else if(goomba[1]->position.x <= 230) {
			goomba_xMove1 = 1;
		}
		if(collisionMap) goomba[1]->Move(Vec2i{goomba_xMove1, 0}, collisionMap);

		if(goomba[1]->GetGravityHandler().IsFalling()) {
			if(collisionMap && goomba[1]->alive) goomba[1]->Move(Vec2i{0, 1}, collisionMap);
		}

		// for(auto *s : game.SpriteSystem().GetTypeList("goomba")) {
		// 	std::cout << "C: " << s->Collides(mario) << ", S: " << !mario->StepsOn(s) << "\n";
		// 	// if(s->Collides(mario) && !mario->StepsOn(s)) {
		// 	// if(mario->StepsOn(s)) {

		// 	// 	mario->position.x = 50;
		// 	// 	mario->position.y = 50;
		// 	// }
		// }
	}

	void SwitchFrom(Game &game, const std::string &other) override {
		if (other == "level1.pipe") {
			mario->position.x = 1142;
			mario->position.y = 100;

			game.GetRenderer().DefaultView.offset.x = mario->position.x - ((640 / game.GetRenderer().ZoomLevel) / 2) - mario->size.x;
			current_border = mario->position.x - ((640 / game.GetRenderer().ZoomLevel) / 2) - mario->size.x;
		}
	}


	void Collisions(Game &game) override {
		if(!mario->alive) {
			mario->alive = true;
		}
		// goomba anim
		int walk;
		const TiledAnimation *moveGAnimation = GetAnimation(
			GOOMBA_ANIMATIONS, ARRAY_COUNT(GOOMBA_ANIMATIONS),
			"goomba_walk",
			walk).unwrap();

		float animGTime = moveGAnimation->totalTime;

		if(goomba[0]->Collides(mario) && goomba[0]->alive) {
			if(mario->StepsOn(goomba[0])) {
				std::cout << "Steps on\n";
				int indx;
				const TiledAnimation *moveGAnimation = GetAnimation(
					GOOMBA_ANIMATIONS, ARRAY_COUNT(GOOMBA_ANIMATIONS),
					"goomba_die",
					indx).unwrap();

				float animGTime = moveGAnimation->totalTime;

				goomba[0]->SetAnimation(indx, 0.0f, animGTime);
				goomba[0]->alive = false;
			} else {
				std::cout << "Collide: " << goomba[0]->Collides(mario) << ", alive: " << goomba[0]->alive << "\n";
				mario->alive = false;
				mario->position.x = 50;
				mario->position.y = 50;
			}
		} else {
			if(goomba[0]->alive) goomba[0]->SetAnimation(walk, 0.0f, animGTime);
		}

		if(goomba[1]->Collides(mario) && goomba[1]->alive) {
			if(mario->StepsOn(goomba[1])) {
				int indx;
				const TiledAnimation *moveGAnimation = GetAnimation(
					GOOMBA_ANIMATIONS, ARRAY_COUNT(GOOMBA_ANIMATIONS),
					"goomba_die",
					indx).unwrap();

				float animGTime = moveGAnimation->totalTime;

				goomba[1]->SetAnimation(indx, 0.0f, animGTime);
				goomba[1]->alive = false;
			} else {
				std::cout << "Collide: " << goomba[0]->Collides(mario) << ", alive: " << goomba[0]->alive << "\n";
				mario->alive = false;
				mario->position.x = 50;
				mario->position.y = 50;
			}
		} else {
			if(goomba[1]->alive) goomba[1]->SetAnimation(walk, 0.0f, animGTime);
		}

		

		int coin_idle;
		const TiledAnimation *moveCAnimation = GetAnimation(
			COIN_ANIMATIONS, ARRAY_COUNT(COIN_ANIMATIONS),
			"coin_idle",
			coin_idle).unwrap();

		float animCTime = moveCAnimation->totalTime;

		if(mario->Collides(coin[0])) {
			int coin_vanish;
			const TiledAnimation *moveCAnimation = GetAnimation(
				COIN_ANIMATIONS, ARRAY_COUNT(COIN_ANIMATIONS),
				"coin_vanish",
				coin_vanish).unwrap();

			float animCTime = moveCAnimation->totalTime;
			coin[0]->SetAnimation(coin_vanish, 0.0f, animCTime);
			coin[0]->alive = false;
		} else {
			if(coin[0]->alive) coin[0]->SetAnimation(coin_idle, 0.0f, animCTime);
		}

		if(mario->Collides(coin[1])) {
			int coin_vanish;
			const TiledAnimation *moveCAnimation = GetAnimation(
				COIN_ANIMATIONS, ARRAY_COUNT(COIN_ANIMATIONS),
				"coin_vanish",
				coin_vanish).unwrap();

			float animCTime = moveCAnimation->totalTime;
			coin[1]->SetAnimation(coin_vanish, 0.0f, animCTime);
			coin[1]->alive = false;
		} else {
			if(coin[1]->alive) coin[1]->SetAnimation(coin_idle, 0.0f, animCTime);
		}

		if(collisionMap) {
			int scale_y = (mario->size.y == 16) ? 0 : 16;
			//std::cout << scale_y << "\n";
			int xp = ((mario->position.x + 8)  / 16); // The center of the player must be "inside" the tile to destroy it, thats why we put +8(half of the x-scale)"
			int yp = ((mario->position.y + scale_y)  / 16) - 1;

			engine::Tilemap *tilemap = game.TilemapSystem().Map().Get(terrain);
			// Destroy brick blocks when they are hit from below
			int dummy;
			// std::cout << "xp, yp:" << yp << ", " << xp << "at: " << tilemap->At(yp, xp) << "\n";
			if(!FilterGridUp(collisionMap, mario->position.x, mario->position.y, mario->size.x, dummy) && tilemap->At(yp, xp) == 3) {
				tilemap->ChangeTile(yp, xp, 8);
				collisionMap->SetAll(yp, xp, engine::GridTile::Empty);
			}
			// Make ? blocks blank blocks when they are hit from below, TODO: they should award the player with a coin
			if(!FilterGridUp(collisionMap, mario->position.x, mario->position.y, mario->size.x, dummy) && tilemap->At(yp, xp) == 0) {
				tilemap->ChangeTile(yp, xp, 6);

				Vec2i topLeftAbove = tilemap->Pixel(yp - 1, xp);

				engine::Texture *mushroomTexture = game.GetTexture(tilemapTextureID).unwrap();
				engine::Sprite *mushroom = new engine::Sprite(game.GetTextureSystem(), &mushroomAnimationSet, tilemapTextureID);
				mushroom->type = "mushroom";

				int defaultIndex;
				const TiledAnimation *moveCAnimation = GetAnimation(
					MUSHROOM_ANIMATIONS, ARRAY_COUNT(MUSHROOM_ANIMATIONS),
					"default",
					defaultIndex).unwrap();

				mushroom->SetAnimation(defaultIndex, 0.f, moveCAnimation->totalTime, SetAnimation_ForceReset | SetAnimation_Looping);
				mushroom->position = topLeftAbove;
				mushroom->size = { 16, 16 };
				mushroom->maxVelocity = { 2, 5};
				mushroom->minVelocity = { 2, 4};

				mushroom->GetGravityHandler().SetOnSolidGround(myOnSolidGround);
				mushroom->GetGravityHandler().SetIsGravityAddicted(true);

				game.SpriteSystem().Add(mushroom);
			}

			for(auto *s : game.SpriteSystem().GetTypeList("mushroom"))
			if(mario->Collides(s) && s->alive) {
				std::cout << "Mushroom hit\n";
				mario->size.y = 32;
				mario->position.y -= 16;
				s->alive = false;
				int defaultIndex;
				const TiledAnimation *moveCAnimation = GetAnimation(
					MUSHROOM_ANIMATIONS, ARRAY_COUNT(MUSHROOM_ANIMATIONS),
					"mushroom_vanish",
					defaultIndex).unwrap();

				s->SetAnimation(defaultIndex, 0.f, moveCAnimation->totalTime, SetAnimation_ForceReset | SetAnimation_Looping);
				break;
			}

			//When mario goes into pipe
			// if(!FilterGridDown(collisionMap, sprite->position.x, sprite->position.y, sprite->size.x, sprite->size.y, dummy) && (tilemap->At(yp+3, xp) == 14 || tilemap->At(yp+3, xp) == 15)) {
			if(game.GetKeyDown(SDL_SCANCODE_S) && (tilemap->At(yp+3, xp) == 14 || tilemap->At(yp+3, xp) == 15)) {
				std::cout << "Change level\n";

				requestLevelSwitchHandler("level1.pipe", game);
				// currentTilemap = levelPipeTilemap;
				// currentLayer = Pipelevel1Layer;
				// collisionMap = ExportGridMap(game.TilemapSystem().Map().Get(currentTilemap), 2);
				// sprite->position.x = 20;
				// sprite->position.y = 16;
				// game.GetRenderer().DefaultView.offset.x = 0;
				current_border = 0;
			}



		}
	}
};