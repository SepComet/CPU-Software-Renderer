#pragma once
#include "Vector3.h"
#include "MathUtil.h"
#include <Matrix4x4.h>

namespace Scene
{
	class Transform
	{
	public:
		Transform() = default;
		Transform(Math::Vector3 position, Math::Vector3 rotation, Math::Vector3 scale) : position(position), rotation(rotation), scale(scale) {}

		Math::Vector3 position = Math::Vector3(0, 0, 0);
		Math::Vector3 rotation = Math::Vector3(0, 0, 0);
		Math::Vector3 scale = Math::Vector3(1, 1, 1);

		Math::Vector3 transform_direction(const Math::Vector3& direction) const
		{
			const Math::Matrix4x4 rotationMatrix = Math::MathUtil::get_rotation_matrix(rotation);
			return rotationMatrix.TransformDirection(direction).to_vector3().normalized();
		}

		Math::Vector3 get_right() const { return transform_direction(Math::Vector3(1.0f, 0.0f, 0.0f)); }
		Math::Vector3 get_left() const { return transform_direction(Math::Vector3(-1.0f, 0.0f, 0.0f)); }
		Math::Vector3 get_up() const { return transform_direction(Math::Vector3(0.0f, 1.0f, 0.0f)); }
		Math::Vector3 get_down() const { return transform_direction(Math::Vector3(0.0f, -1.0f, 0.0f)); }
		Math::Vector3 get_forward() const { return transform_direction(Math::Vector3(0.0f, 0.0f, 1.0f)); }
		Math::Vector3 get_back() const { return transform_direction(Math::Vector3(0.0f, 0.0f, -1.0f)); }
	};
}
