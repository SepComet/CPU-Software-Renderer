#pragma once
#include "Vector3.h"
#include "Matrix4x4.h"

namespace Scene
{
	class Camera
	{
	private:
		Math::Vector3 position = Math::Vector3(0.0f, 0.0f, 0.0f);
		Math::Vector3 target = Math::Vector3(0.0f, 0.0f, -1.0f);
		Math::Vector3 up = Math::Vector3(0.0f, 1.0f, 0.0f);

		float verticalFovRadians = 1.0471975512f;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;

	public:
		Camera() = default;

		Math::Vector3 get_position() const { return position; }
		void set_position(const Math::Vector3& pos) { position = pos; }

		Math::Vector3 get_target() const { return target; }
		void set_target(const Math::Vector3& t) { target = t; }

		Math::Vector3 get_up() const { return up; }
		void set_up(const Math::Vector3& u) { up = u; }

		float get_vertical_fov_radians() const { return verticalFovRadians; }
		void set_vertical_fov_radians(float fov) { verticalFovRadians = fov; }

		float get_near_plane() const { return nearPlane; }
		void set_near_plane(float nearP) { nearPlane = nearP; }

		float get_far_plane() const { return farPlane; }
		void set_far_plane(float farP) { farPlane = farP; }

		Math::Matrix4x4 get_view_matrix() const;

		Math::Matrix4x4 get_projection_matrix(float aspectRatio) const;
	};
}
