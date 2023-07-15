#include "Music.h"
#include "../Resources.h"
#include <spdlog/spdlog.h>

namespace audio
{
	Music::Music(const std::string& name, const std::string& groupName)
	{
		auto res = Resources::Get(name, groupName);
		if (!res)
		{
			spdlog::error("Failed to load music \"{}\" (group {}): Key doesn't exist.", name, groupName);
			return;
		}

		SDL_RWops* rw = SDL_RWFromConstMem(res->Data(), res->Size());
		music = Mix_LoadMUS_RW(rw, true);
		if (!music)
		{
			spdlog::error("Failed to load music \"{}\" (group {}): {}", name, groupName, Mix_GetError());
			return;
		}
	}

	Music::~Music()
	{
		Mix_FreeMusic(music);
	}
}