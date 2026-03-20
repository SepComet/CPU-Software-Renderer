#pragma once
#include "Color.h"
#include "FrameBuffer.h"
#include "DepthBuffer.h"
#include <Vector2.h>

namespace Rasterizer
{
	class Rasterizer
	{
	private:
		Core::FrameBuffer* frameBuffer;
		Core::DepthBuffer* depthBuffer;

		void DrawLineHorizontal(const Math::Vector2Int v0, const Math::Vector2Int v1, const RenderData::Color color);
		void DrawLineVertical(const Math::Vector2Int v0, const Math::Vector2Int v1, const RenderData::Color color);

	public:
		explicit Rasterizer(Core::FrameBuffer* frameBuffer) :frameBuffer(frameBuffer), depthBuffer(nullptr) {}
		explicit Rasterizer(Core::FrameBuffer* frameBuffer, Core::DepthBuffer* depthBuffer) :frameBuffer(frameBuffer), depthBuffer(depthBuffer) {}

		void DrawLine(const Math::Vector2Int v0, const Math::Vector2Int v1, const RenderData::Color color);
	};
}