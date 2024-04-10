struct LevelPipe : public Level {
	engine::TilemapID terrain, background;
	engine::Sprite *coin[3];

	engine::TilemapID GetMainTilemap() override { return terrain; }

	void SetupMario(Game &game) {
		mario = new engine::Sprite(game.GetTextureSystem(), &spritesheet, marioTexture);
		// game->GetRenderer().DefaultView.offset.x = mario->position.x - ((640 / game->GetRenderer().ZoomLevel) / 2) - mario->size.x;
		// current_border = mario->position.x - ((640 / game->GetRenderer().ZoomLevel) / 2) - mario->size.x;

		// std::cout << "DefaultView.offset.x: " << game->GetRenderer().DefaultView.offset.x << ", current_border: " << current_border << "\n";

		auto& gravityHandler = mario->GetGravityHandler();
		gravityHandler.SetOnSolidGround(myOnSolidGround);
		gravityHandler.SetIsGravityAddicted(true);
		gravityHandler.SetOnStartFalling(mario_on_start_falling);
		gravityHandler.SetOnStopFalling(mario_on_stop_falling);
	}

	void SetupCollision(Game &game) {
		const engine::Tilemap *tilemap = game.TilemapSystem().Map().Get(terrain);
		collisionMap = ExportGridMap(tilemap, 2);
	}

	void SetupTilemaps(Game &game) {
		engine::TilemapSystem &ts_pipe = game.TilemapSystem();
		{
			engine::CSVImporter::File file = engine::CSVImporter::Load("media/pipe_lvl1_terrain.csv");
			terrain = ts_pipe.CreateTilemapFromCSV(file, 16).unwrap();
		}
		{
			engine::CSVImporter::File file = engine::CSVImporter::Load("media/pipe_lvl1_bg.csv");
			background = ts_pipe.CreateTilemapFromCSV(file, 16).unwrap();
		}

		layer = engine::Layer("PipeLevel1");
		layer.PushNewTileMapOnLayer(game.TilemapSystem().Map().Get(background));
		layer.PushNewTileMapOnLayer(game.TilemapSystem().Map().Get(terrain));

	}

	void Setup(Game &game) override {
		SetupTilemaps(game);
		SetupCollision(game);
		SetupMario(game);
		SetupCoins(game);
		name = "level1.pipe";
	}

	void SetupCoins(Game &game) {
		coin[0] = new engine::Sprite(game.GetTextureSystem(), &spritesheet2, tilemapTextureID);
		coin[1] = new engine::Sprite(game.GetTextureSystem(), &spritesheet2, tilemapTextureID);
		coin[2] = new engine::Sprite(game.GetTextureSystem(), &spritesheet2, tilemapTextureID);
	}

	void Start(Game &game) override {
		game.GetRenderer().DefaultView.offset.x = 0;
		current_border = 0;

		mario->SetAnimation(0, 0.f, 10.f);
		mario->position = { 50, 50 };
		mario->size = { 16, 16 };
		mario->maxVelocity = { 2, 5};
		mario->minVelocity = { 2, 4};

		game.SpriteSystem().Add(mario);

		coin[0]->type = "coin";
		coin[0]->alive = true;
		coin[0]->SetAnimation(0, 0.f, 10.f);
		coin[0]->position = { 80, 100 };
		coin[0]->size = { 16, 16 };
		coin[0]->maxVelocity = { 2, 5};
		coin[0]->minVelocity = { 2, 4};

		coin[1]->type = "coin";
		coin[1]->alive = true;
		coin[1]->SetAnimation(0, 0.f, 10.f);
		coin[1]->position = { 100, 100 };
		coin[1]->size = { 16, 16 };
		coin[1]->maxVelocity = { 2, 5};
		coin[1]->minVelocity = { 2, 4};

		coin[2]->type = "coin";
		coin[2]->alive = true;
		coin[2]->SetAnimation(0, 0.f, 10.f);
		coin[2]->position = { 120, 100 };
		coin[2]->size = { 16, 16 };
		coin[2]->maxVelocity = { 2, 5};
		coin[2]->minVelocity = { 2, 4};

		game.SpriteSystem().Add(coin[0]);
		game.SpriteSystem().Add(coin[1]);
		game.SpriteSystem().Add(coin[2]);


	}

	void Collisions(Game &game) override {
		int xp = ((mario->position.x + 8)  / 16); // The center of the player must be "inside" the tile to destroy it, thats why we put +8(half of the x-scale)"
		int yp = (mario->position.y  / 16) - 1;

		engine::Tilemap *tilemap = game.TilemapSystem().Map().Get(terrain);

		if(game.GetKeyDown(SDL_SCANCODE_D) && (tilemap->At(yp+3, xp) == 194 || tilemap->At(yp+2, xp+1) == 220)) {
			requestLevelSwitchHandler("level1", game);

		}
	}

	void AI(Game &game) {
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

		if(mario->Collides(coin[2])) {
			int coin_vanish;
			const TiledAnimation *moveCAnimation = GetAnimation(
				COIN_ANIMATIONS, ARRAY_COUNT(COIN_ANIMATIONS),
				"coin_vanish",
				coin_vanish).unwrap();

			float animCTime = moveCAnimation->totalTime;
			coin[2]->SetAnimation(coin_vanish, 0.0f, animCTime);
			coin[2]->alive = false;
		} else {
			if(coin[2]->alive) coin[2]->SetAnimation(coin_idle, 0.0f, animCTime);
		}
	}

	void Cleanup(Game &game) override {
		game.SpriteSystem().Empty();
	}
};
