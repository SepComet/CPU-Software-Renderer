#pragma once
#include "Vector2.h"
#include "BoundingBox.h"
#include "Vertex.h"
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <Vector3.h>
#include <cstdlib>

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

		/// <summary>
		/// 给定屏幕像素坐标，输出该点的面积坐标
		/// </summary>
		/// <param name="pos">要计算的屏幕像素点</param>
		/// <param name="w0">面积坐标的 x 分量（引用）</param>
		/// <param name="w1">面积坐标的 y 分量（引用）</param>
		/// <param name="w2">面积坐标的 z 分量（引用）</param>
		/// <returns>是否计算成功</returns>
		bool get_barycentric(const Math::Vector2& p, float& w0, float& w1, float& w2) const
		{
			using namespace Math;

			const float x0 = v0.position.x;
			const float y0 = v0.position.y;
			const float x1 = v1.position.x;
			const float y1 = v1.position.y;
			const float x2 = v2.position.x;
			const float y2 = v2.position.y;

			const float square2D = (y1 - y2) * (x0 - x2) + (x2 - x1) * (y0 - y2);
			if (std::abs(square2D) < 1e-6f)
			{
				return false;
			}

			w0 = ((y1 - y2) * (p.x - x2) + (x2 - x1) * (p.y - y2)) / square2D;
			w1 = ((y2 - y0) * (p.x - x2) + (x0 - x2) * (p.y - y2)) / square2D;
			w2 = 1.0f - w0 - w1;
			return true;
		}
	};
}