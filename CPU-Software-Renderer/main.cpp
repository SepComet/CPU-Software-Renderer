#include <iostream>
#include <array>
#include <cmath>
#include <SDL.h>
#include "SDL_video.h"
#include "SDL_render.h"
#include <cstdint>
#include "SDL_error.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"
#include "MathUtil.h"
#include "Color.h"
#include "FrameBuffer.h"
#include "Rasterizer.h"
#include "TriangleRasterizer.h"
#include "Triangle.h"
#include "Camera.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_timer.h"
#include <cstdlib>
#include <algorithm>
#include "Vertex.h"
#include "DepthBuffer.h"

const uint32_t SDL_INIT_FLAGS = SDL_INIT_VIDEO;
const int32_t width = 800;
const int32_t height = 600;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;

static void ClearSDLResources()
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	if (renderer != nullptr)
	{
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}

	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	SDL_Quit();
}

static bool EnsureSDLInitialized()
{
	if (SDL_WasInit(SDL_INIT_FLAGS)) return true;

	if (SDL_Init(SDL_INIT_FLAGS) < 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		ClearSDLResources();
		return false;
	}
	return true;
}

static bool EnsureSDLWindow()
{
	if (window != nullptr) return true;

	window = SDL_CreateWindow(
		"SDL Test",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_SHOWN
	);

	if (window == nullptr)
	{
		std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		ClearSDLResources();
		return false;
	}
	return true;
}

static bool EnsureRenderer()
{
	if (renderer != nullptr) return true;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	if (renderer == nullptr)
	{
		std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		ClearSDLResources();
		return false;
	}
	return true;
}

static bool EnsureTexture()
{
	if (texture != nullptr) return true;

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	if (texture == nullptr)
	{
		std::cerr << "Texture could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		ClearSDLResources();
		return false;
	}
	return true;
}

struct ProjectedVertex
{
	Math::Vector3 screen;
	bool visible = false;
};

struct CubeFace
{
	std::array<int, 4> vertices;
};

struct CubeTriangle
{
	std::array<int, 3> vertices;
};

static ProjectedVertex ProjectToScreen(
	const Math::Vector3& vertex,
	const Math::Matrix4x4& mvp,
	const Math::Matrix4x4& viewport)
{
	using namespace Math;

	const Vector4 clip = mvp * Vector4::Point(vertex);
	if (std::abs(clip.w) < 1e-5f)
	{
		return {};
	}

	const float invW = 1.0f / clip.w;
	const float ndcX = clip.x * invW;
	const float ndcY = clip.y * invW;
	const float ndcZ = clip.z * invW;

	if (ndcX < -1.0f || ndcX > 1.0f || ndcY < -1.0f || ndcY > 1.0f || ndcZ < -1.0f || ndcZ > 1.0f)
	{
		return {};
	}

	const Vector4 screen = viewport * Vector4(ndcX, ndcY, ndcZ, 1.0f);
	return { Math::Vector3(screen.x, screen.y, screen.z), true };
}

static bool IsFaceVisible(const CubeFace& face, const std::array<Math::Vector3, 8>& viewSpaceVertices)
{
	using namespace Math;

	const Vector3& v0 = viewSpaceVertices[face.vertices[0]];
	const Vector3& v1 = viewSpaceVertices[face.vertices[1]];
	const Vector3& v2 = viewSpaceVertices[face.vertices[2]];
	const Vector3 faceNormal = (v1 - v0).cross(v2 - v0);
	const Vector3 faceCenter =
		(viewSpaceVertices[face.vertices[0]] +
			viewSpaceVertices[face.vertices[1]] +
			viewSpaceVertices[face.vertices[2]] +
			viewSpaceVertices[face.vertices[3]]) / 4.0f;

	return faceNormal.dot(faceCenter) > 0.0f;
}

static bool IsTriangleVisible(const CubeTriangle& triangle, const std::array<Math::Vector3, 8>& viewSpaceVertices)
{
	using namespace Math;

	const Vector3& v0 = viewSpaceVertices[triangle.vertices[0]];
	const Vector3& v1 = viewSpaceVertices[triangle.vertices[1]];
	const Vector3& v2 = viewSpaceVertices[triangle.vertices[2]];
	const Vector3 faceNormal = (v1 - v0).cross(v2 - v0);
	const Vector3 faceCenter = (v0 + v1 + v2) / 3.0f;

	return faceNormal.dot(faceCenter) > 0.0f;
}

int main(int argc, char* argv[])
{
	if (!EnsureSDLInitialized()) return -1;

	if (!EnsureSDLWindow()) return -1;

	if (!EnsureRenderer()) return -1;

	if (!EnsureTexture()) return -1;

	Core::FrameBuffer* frameBuffer = new Core::FrameBuffer(width, height);
	Core::DepthBuffer* depthBuffer = new Core::DepthBuffer(width, height);
	Rasterizer::Rasterizer rasterizer(frameBuffer, depthBuffer);
	Rasterizer::TriangleRasterizer triangleRasterizer(frameBuffer, depthBuffer);

	Scene::Camera camera;
	camera.transform.position = Math::Vector3(0.0f, 0.0f, 3.0f);
	camera.transform.rotation = Math::Vector3(0.0f, 3.1415926535f, 0.0f);

	const std::array<Math::Vector3, 8> cubeVertices = {
		Math::Vector3(-0.5f, -0.5f, -0.5f),
		Math::Vector3(0.5f, -0.5f, -0.5f),
		Math::Vector3(0.5f, 0.5f, -0.5f),
		Math::Vector3(-0.5f, 0.5f, -0.5f),
		Math::Vector3(-0.5f, -0.5f, 0.5f),
		Math::Vector3(0.5f, -0.5f, 0.5f),
		Math::Vector3(0.5f, 0.5f, 0.5f),
		Math::Vector3(-0.5f, 0.5f, 0.5f)
	};

	const std::array<CubeFace, 6> cubeFaces = {
		CubeFace{ { 0, 3, 2, 1 } },
		CubeFace{ { 4, 5, 6, 7 } },
		CubeFace{ { 0, 4, 7, 3 } },
		CubeFace{ { 1, 2, 6, 5 } },
		CubeFace{ { 0, 1, 5, 4 } },
		CubeFace{ { 3, 7, 6, 2 } }
	};
	const std::array<CubeTriangle, 12> cubeTriangles = {
		CubeTriangle{ { 0, 3, 2 } }, CubeTriangle{ { 0, 2, 1 } },
		CubeTriangle{ { 4, 5, 6 } }, CubeTriangle{ { 4, 6, 7 } },
		CubeTriangle{ { 0, 4, 7 } }, CubeTriangle{ { 0, 7, 3 } },
		CubeTriangle{ { 1, 2, 6 } }, CubeTriangle{ { 1, 6, 5 } },
		CubeTriangle{ { 0, 1, 5 } }, CubeTriangle{ { 0, 5, 4 } },
		CubeTriangle{ { 3, 7, 6 } }, CubeTriangle{ { 3, 6, 2 } }
	};

	const RenderData::Color clearColor(18, 18, 24, 255);
	const RenderData::Color cubeColor(240, 240, 240, 255);
	const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

	bool isRuning = true;
	while (isRuning)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				isRuning = false;
			}

			if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_SPACE)
			{
				isRuning = false;
			}
		}

		frameBuffer->clear(clearColor);
		depthBuffer->clear();

		const float timeSeconds = static_cast<float>(SDL_GetTicks()) * 0.001f;
		const Math::Matrix4x4 model =
			Math::MathUtil::get_rotation_matrix_y(timeSeconds) *
			Math::MathUtil::get_rotation_matrix_x(timeSeconds * 0.6f);
		const Math::Matrix4x4 view = camera.get_view_matrix();
		const Math::Matrix4x4 modelView = view * model;
		const Math::Matrix4x4 projection = camera.get_perspective_projection_matrix(aspectRatio);
		const Math::Matrix4x4 viewport = camera.get_viewport_matrix(static_cast<float>(width), static_cast<float>(height));
		const Math::Matrix4x4 mvp = projection * modelView;

		std::array<Math::Vector3, 8> viewSpaceVertices;
		std::array<ProjectedVertex, 8> projectedVertices;
		for (size_t i = 0; i < cubeVertices.size(); ++i)
		{
			viewSpaceVertices[i] = (modelView * Math::Vector4::Point(cubeVertices[i])).to_vector3();
			projectedVertices[i] = ProjectToScreen(cubeVertices[i], mvp, viewport);
		}

		std::array<bool, 6> visibleFaces = {};
		for (size_t faceIndex = 0; faceIndex < cubeFaces.size(); ++faceIndex)
		{
			visibleFaces[faceIndex] = IsFaceVisible(cubeFaces[faceIndex], viewSpaceVertices);
		}

		std::array<RenderData::Triangle, 12> drawTriangles;
		size_t drawCommandCount = 0;
		for (const CubeTriangle& cubeTriangle : cubeTriangles)
		{
			if (!IsTriangleVisible(cubeTriangle, viewSpaceVertices))
			{
				continue;
			}

			const ProjectedVertex& v0 = projectedVertices[cubeTriangle.vertices[0]];
			const ProjectedVertex& v1 = projectedVertices[cubeTriangle.vertices[1]];
			const ProjectedVertex& v2 = projectedVertices[cubeTriangle.vertices[2]];
			if (!v0.visible || !v1.visible || !v2.visible)
			{
				continue;
			}

			const Math::Vector3& viewV0 = viewSpaceVertices[cubeTriangle.vertices[0]];
			const Math::Vector3& viewV1 = viewSpaceVertices[cubeTriangle.vertices[1]];
			const Math::Vector3& viewV2 = viewSpaceVertices[cubeTriangle.vertices[2]];

			drawTriangles[drawCommandCount++] =
				RenderData::Triangle(
					Scene::Vertex(v0.screen),
					Scene::Vertex(v1.screen),
					Scene::Vertex(v2.screen)
				);
		}

		for (size_t i = 0; i < drawCommandCount; ++i)
		{
			triangleRasterizer.DrawTriangle2D(drawTriangles[i], cubeColor);
		}

		for (size_t faceIndex = 0; faceIndex < cubeFaces.size(); ++faceIndex)
		{
			if (!visibleFaces[faceIndex])
			{
				continue;
			}

			const CubeFace& face = cubeFaces[faceIndex];
			for (size_t edgeOffset = 0; edgeOffset < face.vertices.size(); ++edgeOffset)
			{
				const int startIndex = face.vertices[edgeOffset];
				const int endIndex = face.vertices[(edgeOffset + 1) % face.vertices.size()];
				const ProjectedVertex& start = projectedVertices[startIndex];
				const ProjectedVertex& end = projectedVertices[endIndex];
				if (!start.visible || !end.visible)
				{
					continue;
				}

				rasterizer.DrawLine(
					Math::Vector2(start.screen.x, start.screen.y).to_vector2Int(),
					Math::Vector2(end.screen.x, end.screen.y).to_vector2Int(),
					clearColor);
			}
		}

		SDL_UpdateTexture(texture, nullptr, frameBuffer->get_buffer(), width * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

	ClearSDLResources();
	return 0;
}
