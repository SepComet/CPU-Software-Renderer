#pragma once
#include <cstdint>
#include <utility>
#include <cmath>
#include "Vector3.h"

namespace Math
{
	struct Vector2;
	struct Vector2Int;

	struct Vector2Int
	{
		int32_t x = 0;
		int32_t y = 0;

		Vector2Int() : x(0), y(0) {}
		Vector2Int(int32_t x, int32_t y) : x(x), y(y) {}

		/// <summary>
		/// 交换当前 Vector2Int 对象与另一个 Vector2Int 对象的 x 和 y 的值
		/// </summary>
		/// <param name="other">要交换的对象</param>
		static void swap(Vector2Int& vec1, Vector2Int& vec2)
		{
			std::swap(vec1.x, vec2.x);
			std::swap(vec1.y, vec2.y);
		}
	};

	struct Vector2
	{
		float x = 0.0f;
		float y = 0.0f;

		Vector2() : x(0), y(0) {}
		Vector2(float x, float y) : x(x), y(y) {}
		Vector2(const Vector2Int& other) :x(other.x), y(other.y) {}

		/// <summary>
		/// 交换当前 Vector2 对象与另一个 Vector2 对象的 x 和 y 的值
		/// </summary>
		/// <param name="other">要交换的对象</param>
		static void swap(Vector2& vec1, Vector2& vec2)
		{
			std::swap(vec1.x, vec2.x);
			std::swap(vec1.y, vec2.y);
		}

		Vector2Int to_vector2Int() const
		{
			return Vector2Int(static_cast<int32_t>(std::lround(x)), static_cast<int32_t>(std::lround(y)));
		}

		Vector3 cross(const Vector2& other) const
		{
			return Vector3(0, 0, this->x * other.y - this->y * other.x);
		}
	};
}
