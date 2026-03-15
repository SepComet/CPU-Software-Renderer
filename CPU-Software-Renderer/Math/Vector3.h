#pragma once
#include <cmath>

namespace Math
{
	struct Vector3
	{
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		Vector3() : x(0), y(0), z(0) {}
		Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

		float sqrMagnitude() const
		{
			return x * x + y * y + z * z;
		}

		float magnitude() const
		{
			return std::sqrt(sqrMagnitude());
		}

		Vector3 normalized() const
		{
			const float length = magnitude();
			if (length <= 0.0f)
			{
				return Vector3();
			}

			return *this / length;
		}

		float dot(const Vector3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		// Returns this x other.
		// Order matters:
		// - a.cross(b) == -(b.cross(a))
		// - the result follows the right-hand rule
		// - cross product is not associative
		Vector3 cross(const Vector3& other) const
		{
			return Vector3(
				y * other.z - z * other.y,
				z * other.x - x * other.z,
				x * other.y - y * other.x
			);
		}

		Vector3 operator+ (const Vector3& other) const
		{
			return Vector3(x + other.x, y + other.y, z + other.z);
		}

		Vector3 operator- (const Vector3& other) const
		{
			return Vector3(x - other.x, y - other.y, z - other.z);
		}

		Vector3 operator- () const
		{
			return Vector3(-x, -y, -z);
		}

		Vector3 operator* (float scalar) const
		{
			return Vector3(x * scalar, y * scalar, z * scalar);
		}

		Vector3 operator/ (float scalar) const
		{
			return Vector3(x / scalar, y / scalar, z / scalar);
		}
	};

	inline Vector3 operator* (float scalar, const Vector3& vector)
	{
		return vector * scalar;
	}
}
