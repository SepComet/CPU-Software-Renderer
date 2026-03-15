#pragma once
#include "Vector2.h"
#include "BoundingBox.h"
#include <algorithm>

namespace RenderData
{
	using namespace Math;
	struct Triangle2D
	{
		Vector2 v0;
		Vector2 v1;
		Vector2 v2;
		Triangle2D() : v0(), v1(), v2() {}
		Triangle2D(const Vector2 a, const Vector2 b, const Vector2 c) : v0(a), v1(b), v2(c) {}

		BoundingBox2D get_boundingBox() const
		{
			Vector2 min, max;
			min = Vector2(std::min({ v0.x, v1.x, v2.x }), min.y = std::min({ v0.y, v1.y, v2.y }));
			max = Vector2(std::max({ v0.x, v1.x, v2.x }), max.y = std::max({ v0.y, v1.y, v2.y }));

			return BoundingBox2D(min, max);
		}

		bool ContainsPixel(const Vector2 point) const
		{
			auto cross = [](const Vector2& p1, const Vector2& p2, const Vector2& p3) -> float
				{
					const float x1 = p2.x - p1.x;
					const float y1 = p2.y - p1.y;
					const float x2 = p3.x - p1.x;
					const float y2 = p3.y - p1.y;
					return x1 * y2 - y1 * x2;
				};

			const float c0 = cross(v0, v1, point);
			const float c1 = cross(v1, v2, point);
			const float c2 = cross(v2, v0, point);

			const bool hasNeg = (c0 < 0) || (c1 < 0) || (c2 < 0);
			const bool hasPos = (c0 > 0) || (c1 > 0) || (c2 > 0);

			return !(hasNeg && hasPos);
		}
	};
}