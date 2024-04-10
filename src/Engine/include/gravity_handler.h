#pragma once
#include <functional>
#include <math2d.h>

namespace engine {

class GravityHandler {
public:
	using OnSolidGroundPred = std::function<bool(const Recti&)>;
	using OnStartFalling 	= std::function<void(void)>;
	using OnStopFalling 	= std::function<void(void)>;

protected:
	bool gravityAddicted 	= false;
	bool isFalling 			= false;
	OnSolidGroundPred	onSolidGround = nullptr;
	OnStartFalling 		onStartFalling = nullptr;
	OnStopFalling 		onStopFalling = nullptr;
public:
	template <typename T> void SetOnStartFalling (const T &f) {
		onStartFalling = f;
	}

	template <typename T> void SetOnStopFalling (const T& f) {
		onStopFalling = f;
	}

	template <typename T> void SetOnSolidGround (const T &f) {
		onSolidGround = f;
	}

	// TEST
	inline bool IsFalling() const { return isFalling; }
	inline void SetIsGravityAddicted(bool v) { gravityAddicted = v; }

	void Reset(void) { isFalling = false; }
	void Check(const Recti& r);
};

}