#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

// needed for friend keyword to worm
namespace dev
{
	class Audio;
}

namespace audio
{
	class Music
	{
	public:
		Music(const std::string& name, const std::string& groupName = "default");
		~Music();

	private:
		friend class dev::Audio;

		Mix_Music* music = nullptr;
	};
}