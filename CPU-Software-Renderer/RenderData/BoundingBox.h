#pragma once
#include "Vector2.h"

namespace RenderData
{
	struct BoundingBox2D
	{
		Math::Vector2 min;
		Math::Vector2 max;

		BoundingBox2D() : min(), max() {}
		BoundingBox2D(const Math::Vector2 min, const Math::Vector2 max) : min(min), max(max) {}
	};
}