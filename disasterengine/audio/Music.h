#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

// needed for friend keyword to worm
namespace dev
{
	class Audio;
}

class Resource;
namespace audio
{
	class Music
	{
	public:
		Music();
		~Music();

		bool Load(const uint8_t* data, uint64_t size);

	private:
		friend class dev::Audio;

		Mix_Music* music = nullptr;
	};
}