#pragma once
#include "Matrix4x4.h"
#include "Camera.h"
#include <Vector3.h>

namespace Scene
{
	Math::Matrix4x4 Camera::get_view_matrix() const
	{
		using namespace Math;

		Vector3 forward = (target - position).normalized();
		Vector3 right = forward.cross(up).normalized();
		if (right.magnitude() < 1e-5) right = forward.cross(Vector3(0, 1, 0));
		Vector3 cameraUp = right.cross(forward);

		return Matrix4x4(
			right.x, right.y, right.z, -right.dot(position),
			cameraUp.x, cameraUp.y, cameraUp.z, -cameraUp.dot(position),
			-forward.x, -forward.y, -forward.z, forward.dot(position),
			0, 0, 0, 1
		);
	}

	Math::Matrix4x4 Camera::get_projection_matrix(float aspectRatio) const
	{
		using namespace Math;

		return Matrix4x4(

		);
	}
}
