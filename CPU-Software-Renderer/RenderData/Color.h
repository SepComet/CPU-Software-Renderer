#pragma once
#include <cstdint>

namespace RenderData
{
	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		Color() : r(0), g(0), b(0), a(0) {}
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			this->r = ClampToByte(r);
			this->g = ClampToByte(g);
			this->b = ClampToByte(b);
			this->a = ClampToByte(a);
		}

		uint32_t to_rgba() const
		{
			uint32_t value = (static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 16) | (static_cast<uint32_t>(b) << 8) | (static_cast<uint32_t>(a));
			return value;
		}

		static uint8_t ClampToByte(const int32_t value)
		{
			if (value < 0) return 0;
			if (value > 255) return 255;
			return static_cast<uint8_t>(value);
		}
	};
}