#pragma once
#include <cstdint>
#include <utility>

namespace CustomStructs
{
	struct Vector2Int
	{
		int32_t x;
		int32_t y;
		Vector2Int() : x(0), y(0) {}
		Vector2Int(int32_t x, int32_t y) : x(x), y(y) {}

		/// <summary>
		/// 交换 x 和 y 的值
		/// </summary>
		void swap()
		{
			std::swap(x, y);
		}

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
		float x;
		float y;
		Vector2() : x(0), y(0) {}
		Vector2(float x, float y) : x(x), y(y) {}
		Vector2(const Vector2Int& v) : x(static_cast<float>(v.x)), y(static_cast<float>(v.y)) {}

		/// <summary>
		/// 交换 x 和 y 的值
		/// </summary>
		void swap()
		{
			std::swap(x, y);
		}

		/// <summary>
		/// 交换当前 Vector2 对象与另一个 Vector2 对象的 x 和 y 的值
		/// </summary>
		/// <param name="other">要交换的对象</param>
		static void swap(Vector2& vec1, Vector2& vec2)
		{
			std::swap(vec1.x, vec2.x);
			std::swap(vec1.y, vec2.y);
		}
	};

	struct Vector3Int
	{
		int32_t x;
		int32_t y;
		int32_t z;
		Vector3Int() : x(0), y(0), z(0) {}
		Vector3Int(int32_t x, int32_t y, int32_t z) : x(x), y(y), z(z) {}
	};

	struct Vector3
	{
		float x;
		float y;
		float z;
		Vector3() : x(0), y(0), z(0) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
		Vector3(const Vector3Int& v) : x(static_cast<float>(v.x)), y(static_cast<float>(v.y)), z(static_cast<float>(v.z)) {}
	};

	struct Triangle
	{
		Vector2Int a;
		Vector2Int b;
		Vector2Int c;
		Triangle() : a(), b(), c() {}
		Triangle(const Vector2Int a, const Vector2Int b, const Vector2Int c) : a(a), b(b), c(c) {}

		bool IsInTriangle(const Vector2Int point) const
		{
			auto cross = [](const Vector2Int& p1, const Vector2Int& p2, const Vector2Int& p3) -> int64_t
				{
					const int64_t x1 = static_cast<int64_t>(p2.x) - p1.x;
					const int64_t y1 = static_cast<int64_t>(p2.y) - p1.y;
					const int64_t x2 = static_cast<int64_t>(p3.x) - p1.x;
					const int64_t y2 = static_cast<int64_t>(p3.y) - p1.y;
					return x1 * y2 - y1 * x2;
				};

			const int64_t c0 = cross(a, b, point);
			const int64_t c1 = cross(b, c, point);
			const int64_t c2 = cross(c, a, point);

			const bool hasNeg = (c0 < 0) || (c1 < 0) || (c2 < 0);
			const bool hasPos = (c0 > 0) || (c1 > 0) || (c2 > 0);

			return !(hasNeg && hasPos);
		}
	};

	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
		Color() : r(0), g(0), b(0), a(0) {}
		Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		{
			this->r = ClampToByte(static_cast<int32_t>(r));
			this->g = ClampToByte(static_cast<int32_t>(g));
			this->b = ClampToByte(static_cast<int32_t>(b));
			this->a = ClampToByte(static_cast<int32_t>(a));
		}

		operator uint32_t() const
		{
			return
				(
					(static_cast<uint32_t>(r) << 24) |
					(static_cast<uint32_t>(g) << 16) |
					(static_cast<uint32_t>(b) << 8) |
					(static_cast<uint32_t>(a))
					);
		}

		static uint8_t ClampToByte(const int32_t value)
		{
			if (value < 0) return 0;
			if (value > 255) return 255;
			return static_cast<uint8_t>(value);
		}
	};
}
