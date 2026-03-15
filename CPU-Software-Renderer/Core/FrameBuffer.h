#pragma once
#include "Color.h"
#include "Vector2.h"
#include <cstdint>
#include <vector>

namespace Core
{
	class FrameBuffer
	{
	private:
		int32_t width;
		int32_t height;
		std::vector<uint32_t> buffer;

	public:
		int32_t get_width() const { return width; }

		int32_t get_height() const { return height; }

		size_t total_pixels() const { return buffer.size(); }

		void* get_buffer() const { return (void*)buffer.data(); }

		FrameBuffer(int32_t width, int32_t height) :width(width), height(height), buffer(std::vector<uint32_t>(width * height, 0)) {}

		void clear(const RenderData::Color& color)
		{
			clear(color.to_rgba());
		}

		void clear(const uint32_t color);

		void set_pixel(const int32_t x, const int32_t y, const RenderData::Color& color)
		{
			set_pixel(Math::Vector2Int(x, y), color.to_rgba());
		}

		void set_pixel(const Math::Vector2Int position, const uint32_t color)
		{
			set_pixel(position.x, position.y, color);
		}

		void set_pixel(const int32_t x, const int32_t y, const uint32_t color);
	};
}