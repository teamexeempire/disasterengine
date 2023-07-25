#pragma once
#include "Music.h"
#include "Sample.h"
#include <SDL2/SDL_mixer.h>

namespace dev
{
	class Audio
	{
	public:
		static bool Open();

		static void SetMusicVolume(float volume);
		static void PlayMusic(audio::Music& music, bool loop = false);
		static void StopMusic();

		static int PlaySample(audio::Sample& sample, bool loop = true);
		static void StopChannel(int channel);
	};
}