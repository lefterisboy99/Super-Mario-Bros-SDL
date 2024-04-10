#include <gravity_handler.h>

namespace engine {

void GravityHandler::Check(const Recti& r) {
	if (gravityAddicted) {
		if (onSolidGround == nullptr) return;

		if (onSolidGround(r)) {
			if (isFalling) {
				isFalling = false;
				if (onStopFalling == nullptr) return;
				onStopFalling();
			}
		} else if (!isFalling) {
			isFalling = true;
			if (onStartFalling == nullptr) return;
			onStartFalling();
		}
	}
}

}