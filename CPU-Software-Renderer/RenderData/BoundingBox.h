#pragma once
#include "Vector2.h"

namespace RenderData
{
	struct BoundingBox2D
	{
		Math::Vector2Int min;
		Math::Vector2Int max;

		BoundingBox2D() : min(), max() {}
		BoundingBox2D(const Math::Vector2Int min, const Math::Vector2Int max) : min(min), max(max) {}
	};
}