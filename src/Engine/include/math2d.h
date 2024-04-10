#pragma once
#include <limits.h>
#include <stdint.h>
#include <cmath>

/**
 * Helper Functions
 */

static inline int sgn(int x) {
	if (x < 0) return -1;
	else if (x > 0) return 1;
	else return 0;
}

#pragma pack(push, 1)
template <typename T>
union Vec2 {
	struct { T x; T y;};
	inline Vec2<T> operator*(T m) {return {x*m, y*m};}
};
#pragma pack(pop)

template <typename T>
inline Vec2<T> operator*(T a, Vec2<T> v) {
	return {v.x*a, v.y*a};
}

template <typename T>
static inline Vec2<T> operator+(const Vec2<T> &a, const Vec2<T> &b) { return {a.x + b.x, a.y + b.y}; }
template <typename T>
static inline Vec2<T> operator-(const Vec2<T> &a, const Vec2<T> &b) { return {a.x - b.x, a.y - b.y}; }

typedef Vec2<float> 	Vec2f;
typedef Vec2<uint32_t> 	Vec2u;
typedef Vec2<int32_t> 	Vec2i;

/**
 * Screen-space coordinates rectangle:
 *     1. Use Rectu for spritesheet stuff
 *     2. Use plain Rect for subpixel
 */
#pragma pack(push, 1)
template <typename T>
union Rect {
	struct {
		Vec2<T> pos;
		Vec2<T> size;
	};
	struct {T x, y, w, h;};

	Rect (const Vec2<T>& p, const Vec2<T>& s) {
		this->pos = p;
		this->size = s;
	}

	Rect(const Rect<T> &other) {
		x = other.x;
		y = other.y;
		w = other.w;
		h = other.h;
	}

	Rect(T x, T y, T w, T h) :x(x), y(y), w(w), h(h) {}

	bool Contains(const Rect<T> &b) {
		return 	(b.x > x) &&
				((b.x + b.w) < (x + w)) &&
				(b.y > y) &&
				((b.y + b.h) < (y + h));
	}

};
#pragma pack(pop)

typedef Rect<float> 	Rectf;
typedef Rect<uint32_t> 	Rectu;
typedef Rect<int32_t> 	Recti;