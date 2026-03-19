#pragma once
#include "Triangle.h"
#include "Color.h"
#include "FrameBuffer.h"

namespace Rasterizer
{
	class TriangleRasterizer
	{
	private:
		Core::FrameBuffer& frameBuffer;


	public:
		explicit TriangleRasterizer(Core::FrameBuffer& frameBuffer) :frameBuffer(frameBuffer) {};

		void DrawTriangle2D(const RenderData::Triangle& triangle, const RenderData::Color color);
	};
}