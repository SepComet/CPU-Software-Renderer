#pragma once
#include "Matrix4x4.h"
#include "Camera.h"
#include <Vector3.h>
#include "MathUtil.h"

namespace Scene
{
	Math::Matrix4x4 Camera::get_view_matrix() const
	{
		using namespace Math;

		Vector3 backward = (position - target).normalized();
		Vector3 right = MathUtil::cross(up, backward).normalized();
		if (right.magnitude() < 1e-5) right = MathUtil::cross(Vector3(0, 1, 0), backward);
		Vector3 cameraUp = MathUtil::cross(backward, right);

		return Matrix4x4(
			right.x, right.y, right.z, -right.dot(position),
			cameraUp.x, cameraUp.y, cameraUp.z, -cameraUp.dot(position),
			backward.x, backward.y, backward.z, -backward.dot(position),
			0, 0, 0, 1
		);
	}

	Math::Matrix4x4 Camera::get_orthographic_projection_matrix(float width, float height) const
	{
		using namespace Math;

		const float l = -width * 0.5f;
		const float r = width * 0.5f;
		const float b = -height * 0.5f;
		const float t = height * 0.5f;
		const float n = nearPlane;
		const float f = farPlane;

		return Matrix4x4(
			2.0f / (r - l), 0, 0, -(r + l) / (r - l),
			0, 2.0f / (t - b), 0, -(t + b) / (t - b),
			0, 0, 2.0f / (n - f), -(n + f) / (f - n),
			0, 0, 0, 1
		);
	}

	Math::Matrix4x4 Camera::get_perspective_projection_matrix(float aspectRatio) const
	{
		using namespace Math;

		const float n = nearPlane;
		const float f = farPlane;

		return Matrix4x4(
			1.0f / (aspectRatio * std::tan(verticalFovRadians / 2)), 0, 0, 0,
			0, 1.0f / std::tan(verticalFovRadians / 2), 0, 0,
			0, 0, -(f + n) / (f - n), -(2 * f * n) / (f - n),
			0, 0, -1, 0
		);
	}
}