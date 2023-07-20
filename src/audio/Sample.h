#pragma once
#include <SDL2/SDL_mixer.h>
#include <string>

namespace dev
{
	class Audio;
}

class Resource;
namespace audio
{
	class Sample
	{
	public:
		Sample();
		~Sample();

		bool Load(const uint8_t* data, uint64_t size);

	private:
		friend class dev::Audio;
		Mix_Chunk* chunk = nullptr;
	};
}