#include "DepthBuffer.h"
#include <cstdint>
#include <algorithm>

namespace Core
{
	void DepthBuffer::clear(const uint8_t depth)
	{
		std::fill(buffer.begin(), buffer.end(), depth);
	}


	void DepthBuffer::set_pixel(const int32_t x, const int32_t y, const uint8_t depth)
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
		{
			return;
		}
		// Row-major layout with y = 0 on the first row, matching a top-left screen origin.
		size_t index = static_cast<size_t>(y) * width + x;
		buffer.at(index) = depth;
	}
}
