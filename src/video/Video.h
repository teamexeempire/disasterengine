#pragma once
#include <SDL2/SDL.h>

namespace dev
{
	// Manages window
	class Video
	{
	public:
		static bool Open();
		static void Swap();

		enum VSyncMode
		{
			VSyncAdaptive = -1,
			NoVSync = 0,
			VSync = 1
		};

		static void SetVSync(VSyncMode vsync);

	private:
		inline static SDL_Window* window = nullptr;
		inline static SDL_GLContext glCtx = nullptr;
	};
}