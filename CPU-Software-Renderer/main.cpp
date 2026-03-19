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
#include "Camera.h"
#include "SDL_events.h"
#include "SDL_keycode.h"
#include "SDL_timer.h"
#include <cstdlib>

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
	Math::Vector2Int screen;
	bool visible = false;
};

struct CubeFace
{
	std::array<int, 4> vertices;
	std::array<int, 4> edges;
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
	const float screenX = screen.x;
	const float screenY = screen.y;

	return { Vector2(screenX, screenY).to_vector2Int(), true };
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

	return faceNormal.dot(faceCenter) < 0.0f;
}

int main(int argc, char* argv[])
{
	if (!EnsureSDLInitialized()) return -1;

	if (!EnsureSDLWindow()) return -1;

	if (!EnsureRenderer()) return -1;

	if (!EnsureTexture()) return -1;

	Core::FrameBuffer frameBuffer(width, height);
	Rasterizer::Rasterizer rasterizer(frameBuffer);

	Scene::Camera camera;
	camera.set_position(Math::Vector3(0.0f, 0.0f, 3.0f));
	camera.set_target(Math::Vector3(0.0f, 0.0f, 0.0f));
	camera.set_up(Math::Vector3(0.0f, 1.0f, 0.0f));
	camera.set_near_plane(0.1f);
	camera.set_far_plane(100.0f);

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

	const std::array<std::pair<int, int>, 12> cubeEdges = {
		std::pair<int, int>(0, 1), std::pair<int, int>(1, 2), std::pair<int, int>(2, 3), std::pair<int, int>(3, 0),
		std::pair<int, int>(4, 5), std::pair<int, int>(5, 6), std::pair<int, int>(6, 7), std::pair<int, int>(7, 4),
		std::pair<int, int>(0, 4), std::pair<int, int>(1, 5), std::pair<int, int>(2, 6), std::pair<int, int>(3, 7)
	};
	const std::array<CubeFace, 6> cubeFaces = {
		CubeFace{ { 0, 3, 2, 1 }, { 0, 1, 2, 3 } },
		CubeFace{ { 4, 5, 6, 7 }, { 4, 5, 6, 7 } },
		CubeFace{ { 0, 4, 7, 3 }, { 8, 7, 11, 3 } },
		CubeFace{ { 1, 2, 6, 5 }, { 1, 10, 5, 9 } },
		CubeFace{ { 0, 1, 5, 4 }, { 0, 9, 4, 8 } },
		CubeFace{ { 3, 7, 6, 2 }, { 11, 6, 10, 2 } }
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

		frameBuffer.clear(clearColor);

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

		std::array<bool, 12> visibleEdges = {};
		for (const CubeFace& face : cubeFaces)
		{
			if (!IsFaceVisible(face, viewSpaceVertices))
			{
				continue;
			}

			for (const int edgeIndex : face.edges)
			{
				visibleEdges[edgeIndex] = true;
			}
		}

		for (size_t edgeIndex = 0; edgeIndex < cubeEdges.size(); ++edgeIndex)
		{
			if (!visibleEdges[edgeIndex])
			{
				continue;
			}

			const auto& edge = cubeEdges[edgeIndex];
			const ProjectedVertex& start = projectedVertices[edge.first];
			const ProjectedVertex& end = projectedVertices[edge.second];
			if (!start.visible || !end.visible)
			{
				continue;
			}

			rasterizer.DrawLine(start.screen, end.screen, cubeColor);
		}

		SDL_UpdateTexture(texture, nullptr, frameBuffer.get_buffer(), width * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

	ClearSDLResources();
	return 0;
}
