#pragma once
#include <stdint.h>


namespace engine {
	/**
	 * RGBA8 representation
	 */
	struct Color {
		union {
			struct {
				uint8_t r, g, b, a;
			};
			uint8_t  bytes[4];
			uint32_t dword;
		};

		constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a):r(r), g(g), b(b), a(a) {}
		constexpr Color(uint32_t dword) : dword(dword) {}
		Color() {}

		static constexpr Color Red() 	{ return Color(0xff, 0x00, 0x00, 0xff); }
		static constexpr Color Green() 	{ return Color(0x00, 0xff, 0x00, 0xff); }
		static constexpr Color Blue() 	{ return Color(0x00, 0x00, 0xff, 0xff); }
		static constexpr Color Black() 	{ return Color(0x00, 0x00, 0x00, 0xff); }
		static constexpr Color White() 	{ return Color(0xff, 0xff, 0xff, 0xff); }

		bool operator=(const Color &o) {
			return dword == o.dword;
		}
	};
}