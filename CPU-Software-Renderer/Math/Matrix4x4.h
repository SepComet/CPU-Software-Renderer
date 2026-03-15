#pragma once
#include <cmath>
#include "Vector3.h"
#include "Vector4.h"

namespace Math
{
	class Matrix4x4
	{
	public:
		// 约定：
		// - 图形转换使用列向量语义
		// - 图形变换按列向量语义做，写法是 M * v
		// - 元素访问是 matrix[row][col]
		// - data() 暴露的是 row-major 连续内存
		// - 平移放在最后一列
		Matrix4x4()
		{
			*this = Identity();
		}

		Matrix4x4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			values[0][0] = m00; values[0][1] = m01; values[0][2] = m02; values[0][3] = m03;
			values[1][0] = m10; values[1][1] = m11; values[1][2] = m12; values[1][3] = m13;
			values[2][0] = m20; values[2][1] = m21; values[2][2] = m22; values[2][3] = m23;
			values[3][0] = m30; values[3][1] = m31; values[3][2] = m32; values[3][3] = m33;
		}

		static Matrix4x4 Zero()
		{
			return Matrix4x4(
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f
			);
		}

		static Matrix4x4 Identity()
		{
			return Matrix4x4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
		}

		float* operator[](size_t row)
		{
			return values[row];
		}

		const float* operator[](size_t row) const
		{
			return values[row];
		}

		float* data()
		{
			return &values[0][0];
		}

		const float* data() const
		{
			return &values[0][0];
		}

		Matrix4x4 operator+ (const Matrix4x4& other) const
		{
			Matrix4x4 result = Zero();
			for (size_t row = 0; row < 4; ++row)
			{
				for (size_t col = 0; col < 4; ++col)
				{
					result[row][col] = values[row][col] + other[row][col];
				}
			}

			return result;
		}

		Matrix4x4 operator- (const Matrix4x4& other) const
		{
			Matrix4x4 result = Zero();
			for (size_t row = 0; row < 4; ++row)
			{
				for (size_t col = 0; col < 4; ++col)
				{
					result[row][col] = values[row][col] - other[row][col];
				}
			}

			return result;
		}

		Matrix4x4 operator* (float scalar) const
		{
			Matrix4x4 result = Zero();
			for (size_t row = 0; row < 4; ++row)
			{
				for (size_t col = 0; col < 4; ++col)
				{
					result[row][col] = values[row][col] * scalar;
				}
			}

			return result;
		}

		// 结果为 this x other（调用方 乘以 参数）。
		Matrix4x4 operator* (const Matrix4x4& other) const
		{
			Matrix4x4 result = Zero();
			for (size_t row = 0; row < 4; ++row)
			{
				for (size_t col = 0; col < 4; ++col)
				{
					for (size_t k = 0; k < 4; ++k)
					{
						result[row][col] += values[row][k] * other[k][col];
					}
				}
			}

			return result;
		}

		// 结果为 this x other（调用方 乘以 参数）。
		Vector4 operator* (const Vector4& vector) const
		{
			return Vector4(
				values[0][0] * vector.x + values[0][1] * vector.y + values[0][2] * vector.z + values[0][3] * vector.w,
				values[1][0] * vector.x + values[1][1] * vector.y + values[1][2] * vector.z + values[1][3] * vector.w,
				values[2][0] * vector.x + values[2][1] * vector.y + values[2][2] * vector.z + values[2][3] * vector.w,
				values[3][0] * vector.x + values[3][1] * vector.y + values[3][2] * vector.z + values[3][3] * vector.w
			);
		}

		Vector4 TransformPoint(const Vector3& point) const
		{
			return *this * Vector4::Point(point);
		}

		Vector4 TransformDirection(const Vector3& direction) const
		{
			return *this * Vector4::Direction(direction);
		}

		Matrix4x4 Transposed() const
		{
			Matrix4x4 result = Zero();
			for (size_t row = 0; row < 4; ++row)
			{
				for (size_t col = 0; col < 4; ++col)
				{
					result[row][col] = values[col][row];
				}
			}

			return result;
		}

	private:
		float values[4][4] = {};
	};

	inline Matrix4x4 operator* (float scalar, const Matrix4x4& matrix)
	{
		return matrix * scalar;
	}
}
