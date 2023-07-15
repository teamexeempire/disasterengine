#include "Sample.h"
#include "../Resources.h"
#include <spdlog/spdlog.h>

namespace audio
{
	Sample::Sample(const std::string& name, const std::string& groupName)
	{
		auto res = Resources::Get(name, groupName);
		if (!res)
		{
			spdlog::error("Failed to load sample \"{}\" (group {}): Key doesn't exist.", name, groupName);
			return;
		}

		SDL_RWops* rw = SDL_RWFromConstMem(res->Data(), res->Size());
		chunk = Mix_LoadWAV_RW(rw, true);
		if (!chunk)
		{
			spdlog::error("Failed to load sample \"{}\" (group {}): {}", name, groupName, Mix_GetError());
			return;
		}
	}

	Sample::~Sample()
	{
		Mix_FreeChunk(chunk);
	}

	void Sample::Play(bool loop)
	{
		channel = Mix_PlayChannel(-1, chunk, loop ? -1 : 0);
	}

	void Sample::Stop()
	{
		if (channel == -1)
			return;

		Mix_HaltChannel(channel);
		channel = -1;
	}
}