#include "TriangleRasterizer.h"
#include <Triangle.h>
#include <Color.h>
#include "BoundingBox.h"
#include <Vector2.h>
#include <algorithm>
#include <cstdint>

namespace Rasterizer
{
	void TriangleRasterizer::DrawTriangle2D(const RenderData::Triangle& triangle, const RenderData::Color color)
	{
		using namespace Math;

		auto boundingBox = triangle.get_boundingBox();

		int32_t minX = std::max(0, boundingBox.min.x);
		int32_t maxX = std::min(frameBuffer->get_width() - 1, boundingBox.max.x);
		int32_t minY = std::max(0, boundingBox.min.y);
		int32_t maxY = std::min(frameBuffer->get_height() - 1, boundingBox.max.y);

		for (int x = minX; x <= maxX; x++)
		{
			for (int y = minY; y <= maxY; y++)
			{
				Vector2 samplePoint(x + 0.5f, y + 0.5f);
				float w0 = 0, w1 = 0, w2 = 0;
				if (!triangle.get_barycentric(samplePoint, w0, w1, w2))
				{
					continue;
				}

				if ((w0 < 0) || (w1 < 0) || (w2 < 0))
				{
					continue;
				}

				const float depth =
					w0 * triangle.v0.position.z +
					w1 * triangle.v1.position.z +
					w2 * triangle.v2.position.z;

				if (depthBuffer)
				{
					// 深度越小离相机越近
					if (depthBuffer->get_depth(x, y) < depth)
					{
						continue;
					}

					depthBuffer->set_depth(x, y, depth);
					frameBuffer->set_pixel(x, y, color.to_rgba());
				}
				else
				{
					frameBuffer->set_pixel(x, y, color.to_rgba());
				}
			}
		}
	}
}
