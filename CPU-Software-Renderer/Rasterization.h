#pragma once
#include "StructType.h"
#include <vector>
#include <cstdint>
using namespace CustomStructs;

class Rasterization
{
private:
	int32_t width;
	int32_t height;

public:
	std::vector<uint32_t> frameBuffer;

private:
	void DrawLineHorizontal(Vector2Int v0, Vector2Int v1, Color color);

	void DrawLineVertical(Vector2Int v0, Vector2Int v1, Color color);

public:
	Rasterization(const int32_t width, const int32_t height)
		: width(width), height(height), frameBuffer(std::vector<uint32_t>(static_cast<size_t>(width)* static_cast<size_t>(height))) {}

	void Clear(const Color color);

	void SetPixel(const Vector2Int pos, const Color color);

	void DrawLine(const Vector2Int v0, const Vector2Int v1, Color color);

	void DrawWireTriangle(const Vector2Int v0, const Vector2Int v1, const Vector2Int v2, Color color);

	void DrawFillTriangle(const Vector2Int v0, const Vector2Int v1, const Vector2Int v2, Color color);
};
