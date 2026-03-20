#include "Rasterizer.h"
#include <Vector2.h>
#include "Color.h"
#include <cmath>
#include <cstdint>

namespace Rasterizer
{
	using namespace Math;
	using namespace RenderData;

	void Rasterizer::DrawLineHorizontal(const Vector2Int v0, const Vector2Int v1, const Color color)
	{
		Vector2Int start = v0, end = v1;
		if (v0.x > v1.x)
		{
			start = v1;
			end = v0;
		}

		int32_t dx = end.x - start.x;
		int32_t dy = end.y - start.y;

		int32_t dir = dy < 0 ? -1 : 1;
		dy *= dir;

		if (dx == 0) return;

		int32_t y = start.y;
		int32_t p = 2 * dy - dx;
		for (int32_t x = start.x; x <= end.x; x++)
		{
			frameBuffer->set_pixel(x, y, color.to_rgba());
			// 当 p >= 0 时，取上方像素点
			if (p >= 0)
			{
				y += dir;
				p -= 2 * dx;
			}
			// 当 p < 0 时，取下方像素点
			p += 2 * dy;
		}
	}

	void Rasterizer::DrawLineVertical(const Vector2Int v0, const Vector2Int v1, const Color color)
	{
		Vector2Int start = v0, end = v1;
		if (v0.y > v1.y)
		{
			start = v1;
			end = v0;
		}

		int32_t dx = end.x - start.x;
		int32_t dy = end.y - start.y;

		int32_t dir = dx < 0 ? -1 : 1;
		dx *= dir;

		if (dy == 0) return;

		int32_t x = start.x;
		int32_t p = 2 * dx - dy;
		for (int32_t y = start.y; y <= end.y; y++)
		{
			frameBuffer->set_pixel(x, y, color.to_rgba());
			// 当 p >= 0 时，取上方像素点
			if (p >= 0)
			{
				x += dir;
				p -= 2 * dy;
			}
			// 当 p < 0 时，取下方像素点
			p += 2 * dx;
		}
	}

	/// <summary>
	/// 在给定的起始点和结束点之间绘制一条线段，并使用指定的颜色填充
	/// </summary>
	/// <param name="v0">直线顶点 1</param>
	/// <param name="v1">直线顶点 2</param>
	/// <param name="color">绘制的颜色</param>
	/// <remarks>
	/// bresenham 画线算法
	/// </remarks>
	void Rasterizer::DrawLine(const Vector2Int v0, const Vector2Int v1, const Color color)
	{
		int32_t x0 = v0.x, y0 = v0.y;
		int32_t x1 = v1.x, y1 = v1.y;

		if (std::abs(x1 - x0) > std::abs(y1 - y0))
		{
			DrawLineHorizontal(v0, v1, color);
		}
		else
		{
			DrawLineVertical(v0, v1, color);
		}
	}
}

