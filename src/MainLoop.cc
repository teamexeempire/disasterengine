#include "MainLoop.h"
#include "Input.h"
#include "video/Video.h"
#include "video/Render.h"
#include "audio/Audio.h"

#include <spdlog/spdlog.h>

#define GAMEVER "0.1a"

int MainLoop::Run()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	spdlog::info("--------------------------");
	spdlog::info("DisasterEngine BUILD " __DATE__ " " __TIME__);
	spdlog::info("Version " GAMEVER);
	spdlog::info("(c) Copyright 2023 Team Exe Empire");
	spdlog::info("--------------------------");
	spdlog::info("");

	if (!ModPack::Load("gamedata.mpack"))
	{
		spdlog::error("Giving up, failed to load game's assets...");
		return false;
	}

	if (!dev::Video::Open())
	{
		spdlog::error("Video::Open failed!");
		return 1;
	}

	dev::Video::SetVSync(dev::Video::VSync);

	if (!dev::Audio::Open())
	{
		spdlog::error("Audio::Open failed!");
		return 1;
	}

	spdlog::info("");
	spdlog::info("Now entering main loop....");

	auto music = audio::Music{ "mus_fartzone_chase.ogg" };
	dev::Audio::Play(music, true);

	const double TARGET_FPS = (1000.0 / 60);
	uint64_t freq = SDL_GetPerformanceFrequency();
	uint64_t start = 0, end = 0;
	double delta = 0;

	bool running = true;
	while (running)
	{
		Input::prevKeyStates = Input::keyStates;

		start = SDL_GetPerformanceCounter();

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{;
			switch (e.type)
			{
				case SDL_QUIT:
					running = false;
					break;

				case SDL_KEYDOWN:
					Input::keyStates[e.key.keysym.sym] = true;
					break;

				case SDL_KEYUP:
					Input::keyStates[e.key.keysym.sym] = false;
					break;

				case SDL_WINDOWEVENT:
					switch (e.window.event)
					{
						case SDL_WINDOWEVENT_RESIZED:
							video::Render::Resize(e.window.data1, e.window.data2);
							break;
					}
					break;
			}
		}

		video::Render::Clear(video::Colors::Black);
		video::Render::DrawString(std::format("fps {:.2f}\ndelta {:.4f}", delta * 60.0, delta), 1, 1);
				
		dev::Video::Swap();
		end = SDL_GetPerformanceCounter();
		delta = ((1000 * (end - start)) / (double)freq) / TARGET_FPS;
	}

	return 0;
}

template<typename T>
void MainLoop::SwitchState()
{
	if (state)
	{
		state->Destroy();

		spdlog::info("Exiting {}", state->GetName());
		delete state;
	}

	state = new T();
	state->Init();

	spdlog::info("Entering {}", state->GetName());
}