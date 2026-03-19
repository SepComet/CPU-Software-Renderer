#pragma once
#include "Vector2.h"
#include "BoundingBox.h"
#include "Vertex.h"
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <Vector3.h>

namespace RenderData
{
	struct Triangle
	{
		Scene::Vertex v0;
		Scene::Vertex v1;
		Scene::Vertex v2;
		Triangle() : v0(), v1(), v2() {}
		Triangle(const Scene::Vertex a, const Scene::Vertex b, const Scene::Vertex c) : v0(a), v1(b), v2(c) {}

		BoundingBox2D get_boundingBox() const
		{
			using namespace Math;

			int32_t minX = static_cast<int32_t>(std::floor(std::min({ v0.position.x, v1.position.x, v2.position.x })));
			int32_t maxX = static_cast<int32_t>(std::ceil(std::max({ v0.position.x, v1.position.x, v2.position.x })));
			int32_t minY = static_cast<int32_t>(std::floor(std::min({ v0.position.y, v1.position.y, v2.position.y })));
			int32_t maxY = static_cast<int32_t>(std::ceil(std::max({ v0.position.y, v1.position.y, v2.position.y })));

			Vector2Int min(minX, minY);
			Vector2Int max(maxX, maxY);

			return BoundingBox2D(min, max);
		}

		bool ContainsPixel(const Math::Vector2Int point) const
		{
			using namespace Scene;

			auto cross = [](const Vertex& v1, const Vertex& v2, const Math::Vector2Int& point) -> float
				{
					Scene::Vertex v3(Math::Vector3(point.x, point.y, 0));
					const float x1 = v2.position.x - v1.position.x;
					const float y1 = v2.position.y - v1.position.y;
					const float x2 = v3.position.x - v1.position.x;
					const float y2 = v3.position.y - v1.position.y;
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