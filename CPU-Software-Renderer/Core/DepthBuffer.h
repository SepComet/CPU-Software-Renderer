#pragma once
#include <cstdint>
#include <vector>
#include "Vector2.h"
#include <cmath>

namespace Core
{
	class DepthBuffer
	{
	private:
		int32_t width;
		int32_t height;
		std::vector<float> buffer;

	public:
		int32_t get_width() const { return width; }

		int32_t get_height() const { return height; }

		size_t total_pixels() const { return buffer.size(); }

		void* get_buffer() const { return (void*)buffer.data(); }

		DepthBuffer(int32_t width, int32_t height) :width(width), height(height), buffer(std::vector<float>(width* height, INFINITY)) {}

		void clear(const float depth = INFINITY);

		float get_depth(const Math::Vector2Int position) const { return get_depth(position.x, position.y); }

		float get_depth(const int32_t x, const int32_t y) const;

		void set_depth(const Math::Vector2Int position, const float depth) { set_depth(position.x, position.y, depth); }

		void set_depth(const int32_t x, const int32_t y, const float depth);
	};
};
