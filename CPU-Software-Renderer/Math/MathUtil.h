#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>

namespace Math
{
	class MathUtil
	{
	public:
		static Matrix4x4 get_translate_matrix(const float x, const float y, const float z) { return get_translate_matrix(Vector3(x, y, z)); }

		static Matrix4x4 get_translate_matrix(const Vector3& translation)
		{
			return Matrix4x4(
				1, 0, 0, translation.x,
				0, 1, 0, translation.y,
				0, 0, 1, translation.z,
				0, 0, 0, 1
			);
		}

		static Matrix4x4 get_scale_matrix(const float scale) { return get_scale_matrix(Vector3(scale, scale, scale)); }

		static Matrix4x4 get_scale_matrix(const float x, const float y, const float z) { return get_scale_matrix(Vector3(x, y, z)); }

		static Matrix4x4 get_scale_matrix(const Vector3& scale)
		{
			return Matrix4x4(
				scale.x, 0, 0, 0,
				0, scale.y, 0, 0,
				0, 0, scale.z, 0,
				0, 0, 0, 1
			);
		}

		static Matrix4x4 get_rotation_matrix_x(const float radians)
		{
			const float cosValue = std::cos(radians);
			const float sinValue = std::sin(radians);

			return Matrix4x4(
				1, 0, 0, 0,
				0, cosValue, -sinValue, 0,
				0, sinValue, cosValue, 0,
				0, 0, 0, 1
			);
		}

		static Matrix4x4 get_rotation_matrix_y(const float radians)
		{
			const float cosValue = std::cos(radians);
			const float sinValue = std::sin(radians);

			return Matrix4x4(
				cosValue, sinValue, 0, 0,
				0, 1, 0, 0,
				-sinValue, cosValue, 0, 0,
				0, 0, 0, 1
			);
		}

		static Matrix4x4 get_rotation_matrix_z(const float radians)
		{
			const float cosValue = std::cos(radians);
			const float sinValue = std::sin(radians);

			return Matrix4x4(
				cosValue, -sinValue, 0, 0,
				sinValue, cosValue, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1
			);
		}

		static Matrix4x4 get_rotation_matrix(const Vector3& rotation)
		{
			return get_rotation_matrix_z(rotation.z) * get_rotation_matrix_y(rotation.y) * get_rotation_matrix_x(rotation.x);
		}

		static Matrix4x4 get_rodrigues_rotation_matrix(const Vector3& axis, const float radians)
		{
			const float cosValue = std::cos(radians);
			const float sinValue = std::sin(radians);
			const float oneMinusCos = 1.0f - cosValue;
			const float powX = axis.x * axis.x;
			const float powY = axis.y * axis.y;
			const float powZ = axis.z * axis.z;

			return Matrix4x4(
				1 + 0 + oneMinusCos * (-powZ - powY), 0 + sinValue * (-axis.z) + oneMinusCos * axis.x * axis.y, 0 + sinValue * axis.z + oneMinusCos * axis.x * axis.z, 0,
				0 + sinValue * axis.z + oneMinusCos * axis.x * axis.y, 1 + 0 + oneMinusCos * (-powZ - powX), 0 + sinValue * (-axis.x) + oneMinusCos * axis.y * axis.z, 0,
				0 + sinValue * (-axis.y) + oneMinusCos * axis.x * axis.z, 0 + sinValue * axis.x + oneMinusCos * axis.y * axis.z, 1 + 0 + oneMinusCos * (-powY - powX), 0,
				0, 0, 0, 1
			);
		}

		static Vector3 cross(const Vector3& vec1, const Vector3& vec2)
		{
			return vec1.cross(vec2);
		}
	};
}
