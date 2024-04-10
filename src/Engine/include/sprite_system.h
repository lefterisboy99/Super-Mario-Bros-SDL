#pragma once
#include <math2d.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <functional>
#include <spritesheet.h>
#include <gravity_handler.h>
#include <rendition_system.h>


enum SetAnimationFlag {
    SetAnimation_ForceReset = 1,
    SetAnimation_Looping = 2,
};

constexpr SetAnimationFlag DEFAULT_ANIMATION_FLAG = SetAnimation_Looping;

namespace engine {
    class GridMap;

    bool FilterGridLeft(engine::GridMap *gridmap,int x,int y,int wide,int &snapX);
    bool FilterGridRight(engine::GridMap *gridmap,int x, int y, int width, int height, int &snapX);
    bool FilterGridUp(engine::GridMap *gridmap,int x,int y,int height,int &snapY);
    bool FilterGridDown(engine::GridMap *gridmap,int x,int y,int wide,int height, int &snapY);

    class Sprite {
    public:
        Sprite(TextureSystem &textureSystem, Spritesheet *sheet, TextureID texture)
        :textureSystem(textureSystem) {
            spritesheet = sheet;
            spritesheetTexture = texture;
        }

        inline uint32_t GetCurrentFrame() const {
            if (spritesheet->Num(currentAnimationIndex).unwrap() == 1) return 0;
            return uint32_t((currentAnimationTimer / currentAnimationTime) * float(spritesheet->Num(currentAnimationIndex).unwrap()));
        }

        SDL_Rect GetCurrentFrameRect() const {
            uint32_t frame = GetCurrentFrame();
            Recti *ru = spritesheet->At(currentAnimationIndex, frame).unwrap();

            return {
                ru->x, ru->y,
                ru->w, ru->h,
            };
        }


        void SetAnimation(uint32_t part, float deltaTime, float time, int flag = DEFAULT_ANIMATION_FLAG) {
            bool forceReset = flag & SetAnimation_ForceReset;
            bool loop = flag & SetAnimation_Looping;

            if (part == currentAnimationIndex && !forceReset) return;

            if (part < spritesheet->Num()) {
                currentAnimationIndex = part;
                currentAnimationTimer = deltaTime;
                shouldAnimationLoop = loop;
                currentAnimationTime = time;

            }
        }

        void MoveAnimationTime(float delta) {
            currentAnimationTimer += delta;

            if (currentAnimationTime == 0.0f) return;

            while (shouldAnimationLoop && (currentAnimationTimer >= currentAnimationTime)) {
                currentAnimationTimer -= currentAnimationTime;
            }
        }

        void Display(RenditionSystem& rend) {
            SDL_Rect r = GetCurrentFrameRect();
            Texture *texture = textureSystem.GetTexture(spritesheetTexture).unwrap();
            rend.RenderTexture(
                texture->Handle(),
                position.x, position.y,
                size.x * scale.x, size.y * scale.y, &r);

            rend.RenderRect(position.x, position.y, size.x, size.y, Color::Blue());
        }

        inline Recti GetBox() const {
            return Recti(position, size);
        }


        void Accelerate(Vec2i by, engine::GridMap *collisionMap);
        void Deccelerate(Vec2i by, engine::GridMap *collisionMap);
        void Move(Vec2i by, engine::GridMap *collisionMap);
        bool StepsOn(Sprite *sprite);
        bool Collides(Sprite *sprite);
        bool HitsFromSide(Sprite *sprite);

        void Impulse(Vec2i by, engine::GridMap *collisionMap);

        // TODO: make private
        Vec2i position = {};
        Vec2i velocity = {0, 0};
        Vec2i size = {15, 31};

        Vec2i maxVelocity = {-1, -1}; // always positive
        Vec2i minVelocity = {-1, -1}; // always negative
        Vec2i scale = {1, 1};
        Spritesheet *spritesheet;

        std::string type = "default";
        bool alive = true;

        inline Sprite &SetHasDirectMotion(bool v) { directMotion = true; return *this; }
        inline bool GetHasDirectMotion() const { return directMotion; }
        inline GravityHandler &GetGravityHandler() {return gravity; }

        inline const std::string &Type() const { return type; }
    private:
        // Collision
        GravityHandler gravity;
        bool directMotion = false;
        // Display
        TextureID spritesheetTexture;
        size_t currentAnimationIndex = 0ul;
        float currentAnimationTimer = 0.f;
        float currentAnimationTime = 100.f;
        bool shouldAnimationLoop = true;
        TextureSystem &textureSystem;
    };

    typedef std::list<Sprite *> SpriteList;

    class SpriteSystem {
    public:
    private:
        SpriteList dpyList;
        std::map<std::string, SpriteList> types;

    public:
        void Add(Sprite *sprite);       // ascending z order
        void Remove(Sprite *sprite);

        inline const SpriteList &GetDisplayList() const {return dpyList;}
        inline const SpriteList &GetTypeList(const std::string &typeId) {
            return types[typeId];
        }

        inline void Empty() {
            dpyList.clear();
            types.clear();
        }
    };

}