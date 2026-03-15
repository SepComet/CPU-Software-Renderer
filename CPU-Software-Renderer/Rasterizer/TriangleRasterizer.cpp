#include "TriangleRasterizer.h"
#include <Triangle.h>
#include <Color.h>
#include "BoundingBox.h"
#include <Vector2.h>
#include <algorithm>
#include <cstdint>

namespace Rasterizer
{
	void TriangleRasterizer::DrawTriangle2D(const RenderData::Triangle2D triangle, const RenderData::Color color)
	{
		auto boundingBox = triangle.get_boundingBox();

		int32_t minX = std::max(0, boundingBox.min.x);
		int32_t maxX = std::min(frameBuffer.get_width() - 1, boundingBox.max.x);
		int32_t minY = std::max(0, boundingBox.min.y);
		int32_t maxY = std::min(frameBuffer.get_height() - 1, boundingBox.max.y);

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				if (triangle.ContainsPixel(Math::Vector2(x, y)))
				{
					frameBuffer.set_pixel(Math::Vector2Int(x, y), color.to_rgba());
				}
			}
		}
	}
}