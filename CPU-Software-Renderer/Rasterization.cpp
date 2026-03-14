#include "Rasterization.h"
#include "StructType.h"
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cmath>
using namespace CustomStructs;

void Rasterization::DrawLineHorizontal(Vector2Int v0, Vector2Int v1, Color color)
{
	if (v0.x > v1.x) Vector2Int::swap(v0, v1);

	int32_t dx = v1.x - v0.x;
	int32_t dy = v1.y - v0.y;

	int32_t dir = dy < 0 ? -1 : 1;
	dy *= dir;

	if (dx == 0) return;

	int32_t y = v0.y;
	int32_t p = 2 * dy - dx;
	for (int32_t x = v0.x; x <= v1.x; x++)
	{
		SetPixel(Vector2Int(x, y), color);
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

void Rasterization::DrawLineVertical(Vector2Int v0, Vector2Int v1, Color color)
{
	if (v0.y > v1.y) Vector2Int::swap(v0, v1);

	int32_t dx = v1.x - v0.x;
	int32_t dy = v1.y - v0.y;

	int32_t dir = dx < 0 ? -1 : 1;
	dx *= dir;

	if (dy == 0) return;

	int32_t x = v0.x;
	int32_t p = 2 * dx - dy;
	for (int32_t y = v0.y; y <= v1.y; y++)
	{
		SetPixel(Vector2Int(x, y), color);
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
void Rasterization::DrawLine(Vector2Int v0, Vector2Int v1, Color color)
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

void Rasterization::Clear(const Color color)
{
	uint32_t colorValue = static_cast<uint32_t>(color);
	std::fill(frameBuffer.begin(), frameBuffer.end(), colorValue);
}

void Rasterization::SetPixel(const Vector2Int pos, const Color color)
{
	uint32_t colorValue = static_cast<uint32_t>(color);
	if (pos.x < 0 || pos.y < 0 || pos.x >= width || pos.y >= height) return;
	const size_t pixelIndex = static_cast<size_t>(pos.y) * static_cast<size_t>(width) + static_cast<size_t>(pos.x);
	frameBuffer[pixelIndex] = colorValue;
}

void Rasterization::DrawWireTriangle(const Vector2Int v0, const Vector2Int v1, const Vector2Int v2, Color color)
{
	DrawLine(v0, v1, color);
	DrawLine(v0, v2, color);
	DrawLine(v1, v2, color);
}

void Rasterization::DrawFillTriangle(const Vector2Int v0, const Vector2Int v1, const Vector2Int v2, Color color)
{
	Vector2Int aa, bb;
	aa.x = std::min({ v0.x, v1.x, v2.x });
	aa.y = std::min({ v0.y, v1.y, v2.y });
	bb.x = std::max({ v0.x, v1.x, v2.x });
	bb.y = std::max({ v0.y, v1.y, v2.y });
	Triangle triangle(v0, v1, v2);

	for (int x = aa.x; x <= bb.x; x++)
	{
		for (int y = aa.y; y <= bb.y; y++)
		{
			if (triangle.IsInTriangle(Vector2Int(x, y)))
			{
				SetPixel(Vector2Int(x, y), color);
			}
		}
	}
}
