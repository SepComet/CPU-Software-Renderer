#pragma once
#include "Triangle.h"
#include "Color.h"
#include "FrameBuffer.h"
#include <DepthBuffer.h>

namespace Rasterizer
{
	class TriangleRasterizer
	{
	private:
		Core::FrameBuffer* frameBuffer;
		Core::DepthBuffer* depthBuffer;


	public:
		explicit TriangleRasterizer(Core::FrameBuffer* frameBuffer) :frameBuffer(frameBuffer), depthBuffer(nullptr) {};
		explicit TriangleRasterizer(Core::FrameBuffer* frameBuffer, Core::DepthBuffer* depthBuffer) :frameBuffer(frameBuffer), depthBuffer(depthBuffer) {};

		void DrawTriangle2D(const RenderData::Triangle& triangle, const RenderData::Color color);
	};
}