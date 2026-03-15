#pragma once
#include <cmath>
#include "Vector3.h"

namespace Math
{
	struct Vector4
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
		float w = 0.0f;

		Vector4() : x(0), y(0), z(0), w(0) {}
		Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		static Vector4 Point(const Vector3& point)
		{
			return Vector4(point.x, point.y, point.z, 1.0f);
		}

		static Vector4 Direction(const Vector3& direction)
		{
			return Vector4(direction.x, direction.y, direction.z, 0.0f);
		}

		Vector3 to_vector3() const
		{
			return Vector3(x, y, z);
		}

		float sqrMagnitude() const
		{
			return x * x + y * y + z * z + w * w;
		}

		float magnitude() const
		{
			return std::sqrt(sqrMagnitude());
		}

		Vector4 normalized() const
		{
			const float length = magnitude();
			if (length <= 0.0f)
			{
				return Vector4();
			}

			return *this / length;
		}

		float dot(const Vector4& other) const
		{
			return x * other.x + y * other.y + z * other.z + w * other.w;
		}

		Vector4 operator+ (const Vector4& other) const
		{
			return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
		}

		Vector4 operator- (const Vector4& other) const
		{
			return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
		}

		Vector4 operator* (float scalar) const
		{
			return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
		}

		Vector4 operator/ (float scalar) const
		{
			return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
		}
	};

	inline Vector4 operator* (float scalar, const Vector4& vector)
	{
		return vector * scalar;
	}
}
