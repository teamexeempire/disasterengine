#include "Sample.h"
#include <spdlog/spdlog.h>

namespace audio
{
	Sample::Sample()
	{
	}

	Sample::~Sample()
	{
		Mix_FreeChunk(chunk);
	}

	bool Sample::Load(const uint8_t* data, uint64_t size)
	{
		if (!data)
		{
			spdlog::error("Failed to load sample from resource: Resource is nullptr_t");
			return false;
		}

		SDL_RWops* rw = SDL_RWFromConstMem(data, size);
		chunk = Mix_LoadWAV_RW(rw, true);
		if (!chunk)
		{
			spdlog::error("Failed to load sample from resource: {}", Mix_GetError());
			return false;
		}
	}
}