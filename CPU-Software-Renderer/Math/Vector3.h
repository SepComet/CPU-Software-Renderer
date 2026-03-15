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

		float magnitude() const
		{
			return std::sqrt(x * x + y * y + z * z);
		}
	};
}