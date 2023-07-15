#pragma once
#include "Music.h"
#include <SDL2/SDL_mixer.h>

namespace dev
{
	class Audio
	{
	public:
		static bool Open();

		static void SetVolume(float volume);
		static void Play(audio::Music& music, bool loop = false);
		static void Stop();
	};
}