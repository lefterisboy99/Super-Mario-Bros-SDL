#include "sprite_system.h"
#include <gridmap.h>
#include <fmt/core.h>
namespace engine {

static void Calculate_position_in_element(int x,int y,int *ac_r,int *ac_c,int *subrow,int *subcol){
	*ac_r=y/8;
	*ac_c=x/8;
	*subrow=*ac_r%2;
	*subcol=*ac_c%2;
}

bool FilterGridLeft(engine::GridMap *gridmap,int x,int y,int wide,int &snapX){
	int ac_r,ac_c,subrow,subcol;
	for(int j=y;j<y+wide;j++){
		Calculate_position_in_element(x-1, j,&ac_r,&ac_c,&subrow,&subcol);

		if(gridmap->Get(ac_r/2,ac_c/2,ac_r%2,ac_c%2)== engine::GridTile::Solid){
			snapX = gridmap->GetPositionX_Left(ac_c/2, ac_c%2);
			// fmt::print("FilterGridLeft! {} {} {}\n", x, y, snapX);
			return 0;
		}
	}
	return 1;
}

bool FilterGridRight(engine::GridMap *gridmap,int x, int y, int width, int height, int &snapX){
	int ac_r,ac_c,subrow,subcol;
	for (int j = y; j < y + height; j++) {
		Calculate_position_in_element(x+width+1, j,&ac_r,&ac_c,&subrow,&subcol);

		if(gridmap->Get(ac_r/2,ac_c/2,ac_r%2,ac_c%2)== engine::GridTile::Solid) {
			snapX = gridmap->GetPositionX_Right(ac_c/2, ac_c%2) - width;
			// fmt::print("FilterGridRight! {} {} {}\n", x, y, snapX);
			return 0;
		}
	}
	return 1;
}

bool FilterGridUp(engine::GridMap *gridmap,int x,int y,int height,int &snapY){
	int ac_r,ac_c,subrow,subcol;
	for(int i=x;i<x+height;i++) {
		Calculate_position_in_element(i, y-1,&ac_r,&ac_c,&subrow,&subcol);

		if(gridmap->Get(ac_r/2,ac_c/2,ac_r%2,ac_c%2)== engine::GridTile::Solid){
			snapY = gridmap->GetPositionY_Up(ac_r/2, ac_r%2);
			// fmt::print("FilterGridUp! {} {} {}\n", x, y, snapY);
			return 0;
		}
	}
	return 1;
}

bool FilterGridDown(engine::GridMap *gridmap,int x,int y,int wide,int height, int &snapY){
	int ac_r,ac_c,subrow,subcol;
	for(int i = x; i < x + wide; i++) {
		Calculate_position_in_element(i, y+height+1,&ac_r,&ac_c,&subrow,&subcol);

		if (gridmap->Get(ac_r/2,ac_c/2,ac_r%2,ac_c%2)== engine::GridTile::Solid) {

			snapY = gridmap->GetPositionY_Down(ac_r/2, ac_r%2) - height - 1;
			// fmt::print("FilterGridDown! {} {} {}\n", x, y, snapY);
			return 0;
		}
	}
	return 1;
}

void Sprite::Accelerate(Vec2i by, engine::GridMap *collisionMap) {
	velocity.x += by.x;
	velocity.y += by.y;

	if (velocity.x > maxVelocity.x) velocity.x = maxVelocity.x;
	if (velocity.y > maxVelocity.y) velocity.y = maxVelocity.y;

	if (velocity.x < -minVelocity.x) velocity.x = -minVelocity.x;
	if (velocity.y < -minVelocity.y) velocity.y = -minVelocity.y;

	Sprite::Move(velocity, collisionMap);
}

void Sprite::Deccelerate(Vec2i by, engine::GridMap *collisionMap) {
	by.x = abs(by.x);
	by.y = abs(by.y);

	Vec2i decelValue = {
		-sgn(velocity.x)*by.x,
		-sgn(velocity.y)*by.y
	};
	Accelerate(decelValue, collisionMap);
}

void Sprite::Impulse(Vec2i by, engine::GridMap *collisionMap) {
	if (!(by.x) == 0) velocity.x = by.x;
	if (!(by.y) == 0) velocity.y = by.y;
}

void Sprite::Move(Vec2i by, engine::GridMap *collisionMap) {

	if (directMotion) {
		position.x += by.x;
		position.y += by.y;
		return;
	}

	Recti r = GetBox();
	gravity.Check(r);

	if(by.x<0){
		int snapX;
		if(FilterGridLeft(collisionMap, position.x, position.y, size.y,snapX)){
			position.x += by.x;
		}else{
			position.x = snapX;
		}
	}
	if(by.x>0){
		int snapX;
		if(FilterGridRight(collisionMap, position.x, position.y, size.x, size.y,snapX)){
			position.x += by.x;
		}else{
			position.x = snapX;
		}
	}
	if(by.y>0){
		int snapY;
		if(FilterGridDown(collisionMap, position.x, position.y, size.x, size.y, snapY)) {
			position.y += by.y;
		} else {
			position.y = snapY;
		}
	}
	if(by.y<0){
		int snapY;
		if(FilterGridUp(collisionMap, position.x, position.y, size.x,snapY)){
			position.y += by.y;
		}else{
			position.y = snapY;
		}
	}

	// Check Gridmap bounds
	// if (position.x < 0) position.x = 0;
	// if (position.x > collisionMap->Width()) position.x = collisionMap->Width() - size.x;

	// if (position.y < 0) position.y = 0;
	// if (position.y > collisionMap->Height()) position.y = collisionMap->Height() - size.y;

}

static bool check_corner_br_tl_bool_step(int p_x, int p_y, int l_x, int l_y, int l_x1, int l_y1) {
	//std::cout << "br_tl -> p(x,y): " << p_x << ", " << p_y << "\n";
	if (p_y < l_y && p_x >= l_x && p_y >= l_y1 && p_x <= l_x1) return true;
	return false;
}

static bool check_corner_bl_tr_bool_step(int p_x, int p_y, int l_x, int l_y, int l_x1, int l_y1) {
	//std::cout << "bl_tr -> p(x,y): " << p_x << ", " << p_y <<"\n";

	if (p_y < l_y && p_x <= l_x && p_y >= l_y1 && p_x >= l_x1) return true;
	return false;
}

bool Sprite::StepsOn(Sprite *sprite) {
	bool tmp = false;
	tmp = check_corner_br_tl_bool_step(position.x + size.x, // bottom right x player
								  position.y - size.y + 10, // bottom right y player
								  sprite->position.x, // top left x sprite
								  sprite->position.y, // top left y sprite
								  sprite->position.x + sprite->size.x,  // bottom right x sprite
								  sprite->position.y - sprite->size.y); // bottom right x sprite
	if (tmp) return false;

	tmp = check_corner_bl_tr_bool_step(position.x, // bottom left x player
								  position.y - size.y + 10, // bottom left y player
								  sprite->position.x + sprite->size.x, // top right x sprite
								  sprite->position.y, // top right y sprite
								  sprite->position.x, // bottom left x sprite
								  sprite->position.y - sprite->size.y); // bottom left y sprite
	if (tmp) return false;

	return true;
}

static bool check_corner_br_tl_bool(int p_x, int p_y, int l_x, int l_y, int l_x1, int l_y1) {
	if (p_y <= l_y && p_x >= l_x && p_y >= l_y1 && p_x <= l_x1) return true;
	return false;
}

static bool check_corner_tr_bl_bool(int p_x, int p_y, int l_x, int l_y, int l_x1, int l_y1) {
	if (p_y >= l_y && p_x >= l_x && p_y <= l_y1 && p_x <= l_x1) return true;
	return false;
}

static bool check_corner_tl_br_bool(int p_x, int p_y, int l_x, int l_y, int l_x1, int l_y1) {
	if (p_y >= l_y && p_x <= l_x && p_y <= l_y1 && p_x >= l_x1) return true;
	return false;
}

static bool check_corner_bl_tr_bool(int p_x, int p_y, int l_x, int l_y, int l_x1, int l_y1) {
	if (p_y <= l_y && p_x <= l_x && p_y >= l_y1 && p_x >= l_x1) return true;
	return false;
}

static bool check_side_l_r(float p2_x, float p2_y, float p1_x, float p1_y, float l3_x, float l3_y, float l0_x, float l0_y, float l1_x, float l1_y)
{
	if (p2_y <= l3_y && p1_y >= l0_y && p2_x > l3_x && p1_x > l0_x && p1_x < l1_x) {
		return true;
	}
	return false;
}

static bool check_side_r_l(float p0_x, float p0_y, float p3_x, float p3_y, float l1_x, float l1_y, float l2_x, float l2_y, float l0_x, float l0_y)
{
	if (p0_y >= l1_y && p3_y <= l2_y && p0_x < l1_x  && p3_x < l2_x  && p0_x > l0_x) {
		return true;
	}
	return false;
}

static bool check_side_b_t(float p0_x, float p0_y, float p1_x, float p1_y, float l3_x, float l3_y, float l2_x, float l2_y, float l0_x, float l0_y)
{
	if (p0_x <= l3_x && p0_y > l3_y && p1_x >= l2_x && p1_y > l2_y && p0_y < l0_y) {
		return true;
	}
	return false;
}

static bool check_side_t_b(float p2_x, float p2_y, float p3_x, float p3_y, float l1_x, float l1_y, float l0_x, float l0_y, float l3_x, float l3_y)
{
	if (p3_x <= l0_x && p3_y < l0_y && p2_y < l1_y && p2_x >= l1_x && p3_y > l3_y) {
		return true;
	}
	return false;
}

bool Sprite::Collides(Sprite *sprite) {
	bool tmp = false;
	tmp = check_corner_br_tl_bool(position.x + size.x, // bottom right x player
								  position.y - size.y, // bottom right y player
								  sprite->position.x, // top left x sprite
								  sprite->position.y, // top left y sprite
								  sprite->position.x + sprite->size.x,  // bottom right x sprite
								  sprite->position.y - sprite->size.y); // bottom right x sprite
	if (tmp) return true;

	tmp = check_corner_tr_bl_bool(position.x + size.x, // top right x player
								  position.y, // top right y player
								  sprite->position.x, // bottom left x sprite
								  sprite->position.y - sprite->size.y, // bottom left y sprite
								  sprite->position.x + sprite->size.x, // top right x player
								  sprite->position.y); // top right y player
	if (tmp) return true;

	tmp = check_corner_tl_br_bool(position.x,  // top left x player
								  position.y,  // top left y player
								  sprite->position.x + sprite->size.x, // bottom right x sprite 
								  sprite->position.y - sprite->size.y, // bottom right y sprite
								  sprite->position.x, // top left x sprite
								  sprite->position.y); // top left y sprite
	if (tmp) return true;

	tmp = check_corner_bl_tr_bool(position.x, // bottom left x player
								  position.y - size.y, // bottom left y player
								  sprite->position.x + sprite->size.x, // top right x sprite
								  sprite->position.y, // top right y sprite
								  sprite->position.x, // bottom left x sprite
								  sprite->position.y - sprite->size.y); // bottom left y sprite
	if (tmp) return true;

	tmp = check_side_l_r(
			position.x + size.x,
			position.y,
			position.x + size.x,
			position.y - size.y,
			sprite->position.x,
			sprite->position.y,
			sprite->position.x,
			sprite->position.y - sprite->size.y,
			sprite->position.x + sprite->size.x,
			sprite->position.y - sprite->size.y);
		if (tmp != false) {
			return true;
		}

		tmp = check_side_r_l(
			position.x,
			position.y - size.y,
			position.x,
			position.y,
			sprite->position.x + sprite->size.x,
			sprite->position.y - sprite->size.y,
			sprite->position.x + sprite->size.x,
			sprite->position.y,
			sprite->position.x,
			sprite->position.y - sprite->size.y);
		if (tmp != false) {
			return true;
		}

		tmp = check_side_t_b(
			position.x + size.x,
			position.y,
			position.x,
			position.y,
			sprite->position.x + sprite->size.x,
			sprite->position.y - sprite->size.y,
			sprite->position.x,
			sprite->position.y - sprite->size.y,
			sprite->position.x,
			sprite->position.y);
		if (tmp != false) {
			return true;
		}

		tmp = check_side_b_t(
			position.x,
			position.y - size.y,
			position.x + size.x,
			position.y - size.y,
			sprite->position.x,
			sprite->position.y,
			sprite->position.x + sprite->size.x,
			sprite->position.y,
			sprite->position.x,
			sprite->position.y - sprite->size.y);
		if (tmp != false) {
			return true;
		}

	return false;
}

void SpriteSystem::Add(Sprite *sprite) {
	types[sprite->Type()].push_back(sprite);
	dpyList.push_back(sprite);

	// if (types.find(sprite->type)) {


	// } else {
	// 	types[sprite->type].push_back(sprite);
	// }
}

void SpriteSystem::Remove(Sprite *sprite) {
	// remove from map
	{
		auto findResult = types.find(sprite->Type());

		if (findResult == types.end()) return;

		SpriteList &list = findResult->second;
		auto it = list.begin();

		while (it != list.end()) {
			Sprite *qSprite = *it;
			if (qSprite == sprite) {
				list.erase(it);
				return;
			}

			it++;
		}
	}

	// remove from dpyList
	{
		auto it = dpyList.begin();

		while (it != dpyList.end()) {
			if (*it == sprite) {
				dpyList.erase(it);
				return;
			}
			it++;
		}

	}
}


}