#include "FrameBuffer.h"
#include <cstdint>
#include <iostream>
#include <algorithm>
using namespace Math;

namespace Core
{
	void FrameBuffer::clear(const uint32_t color)
	{
		std::fill(buffer.begin(), buffer.end(), color);
	}


	void FrameBuffer::set_pixel(const int32_t x, const int32_t y, const uint32_t color)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
		{
			return;
		}
		size_t index = static_cast<size_t>(y) * width + x;
		buffer.at(index) = color;
	}
}