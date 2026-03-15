#include <iostream>
#include <SDL.h>
#include "SDL_video.h"
#include "SDL_render.h"
#include <cstdint>
#include "SDL_error.h"
#include "Vector2.h"
#include "Color.h"
#include "FrameBuffer.h"
#include "Rasterizer.h"
#include "SDL_events.h"
#include "SDL_keycode.h"

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


int main(int argc, char* argv[])
{
	if (!EnsureSDLInitialized()) return -1;

	if (!EnsureSDLWindow()) return -1;

	if (!EnsureRenderer()) return -1;
	
	if (!EnsureTexture()) return -1;

	Core::FrameBuffer frameBuffer(width, height);
	Rasterizer::Rasterizer rasterizer(frameBuffer);

	Math::Vector2Int v0(100, 100);
	Math::Vector2Int v1(300, 100);
	Math::Vector2Int v2(300, 400);
	RenderData::Color color0(255, 0, 0, 255);
	RenderData::Color color1(0, 255, 0, 255);
	RenderData::Color color2(0, 0, 255, 255);
	rasterizer.DrawLine(v0, v1, color0);
	rasterizer.DrawLine(v1, v2, color1);
	rasterizer.DrawLine(v2, v0, color2);

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

		SDL_UpdateTexture(texture, nullptr, frameBuffer.get_buffer(), width * sizeof(uint32_t));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

	ClearSDLResources();
	return 0;
}

