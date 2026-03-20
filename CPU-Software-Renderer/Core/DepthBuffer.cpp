#include "DepthBuffer.h"
#include <cstdint>
#include <algorithm>
#include <cmath>

namespace Core
{
	void DepthBuffer::clear(const float depth)
	{
		std::fill(buffer.begin(), buffer.end(), depth);
	}

	float DepthBuffer::get_depth(const int32_t x, const int32_t y) const
	{
		if (x < 0 || x >= width || y < 0 || y >= height)
		{
			return INFINITY;
		}
		// Row-major layout with y = 0 on the first row, matching a top-left screen origin.
		size_t index = static_cast<size_t>(y) * width + x;
		return buffer.at(index);
	}

	void DepthBuffer::set_depth(const int32_t x, const int32_t y, const float depth)
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
