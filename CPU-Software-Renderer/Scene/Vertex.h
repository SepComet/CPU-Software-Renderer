#pragma once
#include "Vector3.h"

namespace Scene
{
	class Vertex
	{
	public:
		Vertex() = default;
		Vertex(Math::Vector3 position) :position(position) {}

		Math::Vector3 position = Math::Vector3(0, 0, 0);
	};
}