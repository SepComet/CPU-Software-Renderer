#pragma once
#include <cstdint>
#include <vector>
#include "Color.h"
#include "Vector2.h"

namespace Core
{
	class DepthBuffer
	{
	private:
		int32_t width;
		int32_t height;
		std::vector<uint8_t> buffer;

	public:
		int32_t get_width() const { return width; }

		int32_t get_height() const { return height; }

		size_t total_pixels() const { return buffer.size(); }

		void* get_buffer() const { return (void*)buffer.data(); }

		DepthBuffer(int32_t width, int32_t height) :width(width), height(height), buffer(std::vector<uint8_t>(width* height, 0)) {}

		void clear(const uint8_t depth);

		void set_pixel(const Math::Vector2Int position, const uint8_t depth)
		{
			set_pixel(position.x, position.y, depth);
		}

		void set_pixel(const int32_t x, const int32_t y, const uint8_t depth);
	};
};
