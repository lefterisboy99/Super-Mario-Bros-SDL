#pragma once

#include <spritesheet.h>
#include <texture_system.h>
#include <math2d.h>
#include <string>

struct TiledAnimation {
	Vec2i startFrame; 	// x, y! not row column!
	int numFrames;
	float totalTime;
	std::string name;
};

static TiledAnimation MARIO_SMALL_ANIMATIONS[] = {
	{
		{0, 3},
		1,
		0.f,
		"small_idle"
	},

	{
		{1, 3},
		2,
		0.2f,
		"small_move"
	},

	{
		{5, 3},
		1,
		0.2f,
		"small_jump"
	},

	{
		{6, 3},
		1,
		0.2f,
		"small_die"
	},

};

static TiledAnimation GOOMBA_ANIMATIONS[] = {
	// NOTE: if we had only one animation film, the animation didnt work. Possible bug, or im missing something
	{
		{17, 1},
		2,
		0.2f,
		"goomba_walk2"
	},

	{
		{17, 1},
		2,
		0.5f,
		"goomba_walk"
	},

	{
		{19, 1},
		1,
		0.5f,
		"goomba_die"
	},


};

static TiledAnimation COIN_ANIMATIONS[] = {
	// NOTE: if we had only one animation film, the animation didnt work. Possible bug, or im missing something
	{
		{17, 1},
		2,
		0.2f,
		"placeholder"
	},

	{
		{0, 1},
		3,
		0.3f,
		"coin_idle"
	},

	{
		{8, 0},
		1,
		0.5f,
		"coin_vanish"
	},


};

static TiledAnimation MUSHROOM_ANIMATIONS[] = {
	{
		{8, 8},
		3,
		0.3f,
		"default"
	},

	{
		{8, 0},
		1,
		0.5f,
		"mushroom_vanish"
	},
};

static Result<TiledAnimation*> GetAnimation(TiledAnimation *animations, int numAnimations, std::string name, int &index) {
	for (int i = 0; i < numAnimations; ++i) {
		if (animations[i].name == name) {
			index = i;
			return ok(&animations[i]);
		}
	}
	return err("Not found");
}

static void BuildSpritesheet(engine::Texture *texture, engine::Spritesheet &sheet, TiledAnimation *animations, int numAnimations, Vec2i tileSize) {
	int numColumns = texture->Width() / tileSize.x;

	for (int i = 0; i < numAnimations; ++i) {
		TiledAnimation *anim = animations + i;

		engine::SpritesheetPart part;

		Vec2i currentFrame = anim->startFrame;

		for (int frame = 0; frame < anim->numFrames; ++frame) {


			Recti r({currentFrame.x * tileSize.x, currentFrame.y * tileSize.y }, tileSize);
			part.Add(r);

			currentFrame.x += 1;
			if (currentFrame.x == numColumns) {
				currentFrame.x = 0;
				currentFrame.y += 1;
			}
		}

		sheet.Add(std::move(part));
	}
}