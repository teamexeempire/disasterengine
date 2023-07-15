#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

namespace audio
{
	class Sample
	{
	public:
		Sample(const std::string& name, const std::string& groupName = "default");
		~Sample();

		void Play(bool loop = false);
		void Stop();

	private:
		Mix_Chunk* chunk;
		int channel = -1;
	};
}