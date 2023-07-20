#include "Audio.h"
#include <spdlog/spdlog.h>

namespace dev
{
    bool Audio::Open()
    {
        auto flags = Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_FLAC);

        spdlog::info("SDL_Mixer supported formats:");
        spdlog::info("  OGG: {}", (flags & MIX_INIT_OGG) != 0);
        spdlog::info("  MP3: {}", (flags & MIX_INIT_MP3) != 0);
        spdlog::info("  FLAC: {}", (flags & MIX_INIT_FLAC) != 0);

        if (Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 1024) < 0)
        {
            spdlog::error("Failed to open audio device: {}", SDL_GetError());
            return false;
        }

        int freq, channels;
        Uint16 fmt;
        Mix_QuerySpec(&freq, &fmt, &channels);

        spdlog::info("Audio device:");
        spdlog::info("  Frequency: {}hz", freq);
        spdlog::info("  Channels: {}", channels);
        spdlog::info("  Format: {}", fmt);

        return true;
    }

    void Audio::SetMusicVolume(float volume)
    {
        Mix_VolumeMusic(volume * MIX_MAX_VOLUME);
    }

    void Audio::PlayMusic(audio::Music& music, bool loop)
    {
        Mix_PlayMusic(music.music, loop ? -1 : 0);
    }

    void Audio::StopMusic()
    {
        Mix_HaltMusic();
    }

    int Audio::PlaySample(audio::Sample& sample, bool loop)
    {
        return Mix_PlayChannel(-1, sample.chunk, loop ? -1 : 0);
    }

    void Audio::StopChannel(int channel)
    {
        Mix_HaltChannel(channel);
    }
}
