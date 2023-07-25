#include "Video.h"
#include "GLExt.h"
#include "Render.h"
#include <spdlog/spdlog.h>
#include <SDL2/SDL_image.h>

namespace dev
{
	bool Video::Open()
	{
		window = SDL_CreateWindow("DisasterEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 480 * 2, 270 * 2, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
		if (!window)
		{
			spdlog::error("Failed to create window: {}", SDL_GetError());
			return false;
		}

		glCtx = SDL_GL_CreateContext(window);
		if (!glCtx)
		{
			spdlog::error("Failed to GL context: {}", SDL_GetError());
			return false;
		}

		spdlog::info("OpenGL version: {}", (char*)glGetString(GL_VERSION));
		spdlog::info("OpenGL extensions:");
		if (!video::GLExt::LoadGLFunctions())
			return false;

		auto flags = IMG_Init(IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP);

		spdlog::info("SDL_Image supported formats:");
		spdlog::info("  PNG: {}", (flags & IMG_INIT_PNG) != 0);
		spdlog::info("  TIF: {}", (flags & IMG_INIT_TIF) != 0);
		spdlog::info("  WEBP: {}", (flags & IMG_INIT_WEBP) != 0);

		return true;
	}

	void Video::Swap()
	{
		SDL_GL_SwapWindow(window);
	}

	void Video::SetVSync(VSyncMode mode)
	{
		SDL_GL_SetSwapInterval(mode);
	}
}