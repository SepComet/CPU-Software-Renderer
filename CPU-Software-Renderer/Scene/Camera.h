#pragma once
#include "Matrix4x4.h"
#include "Transform.h"

namespace Scene
{
	class Camera
	{
	public:
		Camera() = default;

		Transform transform;

		float verticalFovRadians = 1.0471975512f;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;

		Math::Matrix4x4 get_view_matrix() const;

		Math::Matrix4x4 get_orthographic_projection_matrix(float width, float height) const;

		Math::Matrix4x4 get_perspective_projection_matrix(float aspectRatio) const;

		Math::Matrix4x4 get_viewport_matrix(float width, float height) const;
	};
}
