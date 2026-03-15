#include "TriangleRasterizer.h"
#include <Triangle.h>
#include <Color.h>
#include "BoundingBox.h"
#include <Vector2.h>
using namespace RenderData;

namespace Rasterizer
{
	void TriangleRasterizer::DrawTriangle2D(const Triangle2D triangle, const Color color)
	{
		auto boundingBox = triangle.get_boundingBox();

		for (int x = boundingBox.min.x; x <= boundingBox.max.x; x++)
		{
			for (int y = boundingBox.min.y; y <= boundingBox.max.y; y++)
			{
				if (triangle.ContainsPixel(Vector2(x, y)))
				{
					frameBuffer.set_pixel(Vector2Int(x, y), color.to_rgba());
				}
			}
		}
	}
}