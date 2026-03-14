#include <vector>
#include <iostream>
#include <SDL.h>
#include "Rasterization.h"
#include "StructType.h"
#include "SDL_video.h"
#include "SDL_render.h"
#include <cstdint>
#include "SDL_error.h"
#include <cmath>

const uint32_t SDL_INIT_FLAGS = SDL_INIT_VIDEO;
const int width = 800;
const int height = 600;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

static void ClearSDLResources()
{
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


int main(int argc, char* argv[])
{
	if (!EnsureSDLInitialized()) return -1;

	if (!EnsureSDLWindow()) return -1;

	if (!EnsureRenderer()) return -1;

	SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	Rasterization* rasterization = new Rasterization(width, height);

	CustomStructs::Vector2Int v0(100, 100);
	CustomStructs::Vector2Int v1(300, 100);
	CustomStructs::Vector2Int v2(300, 400);
	CustomStructs::Color color1(255, 0, 0, 255);
	rasterization->DrawFillTriangle(v0, v1, v2, color1);

	while (true)
	{
		SDL_UpdateTexture(texture, nullptr, rasterization->frameBuffer.data(), width * sizeof(uint32_t));

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

	ClearSDLResources();
	return 0;
}

