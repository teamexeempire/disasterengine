#include "Music.h"
#include <spdlog/spdlog.h>

namespace audio
{
	bool Music::Load(const uint8_t* data, uint64_t size)
	{
		if (!data)
		{
			spdlog::error("Failed to load music from resource: resource is nullptr_t");
			return false;
		}

		SDL_RWops* rw = SDL_RWFromConstMem(data, size);
		music = Mix_LoadMUS_RW(rw, true);
		if (!music)
		{
			spdlog::error("Failed to load music from resource: {}", Mix_GetError());
			return false;
		}

		return true;
	}

	Music::Music()
	{
	}

	Music::~Music()
	{
		Mix_FreeMusic(music);
	}
}