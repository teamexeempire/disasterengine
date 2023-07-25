#include "MainLoop.h"
#include "Input.h"
#include "res/Resources.h"
#include "lua/Lua.h"
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

	if (!dev::Audio::Open())
	{
		spdlog::error("Audio::Open failed!");
		return 1;
	}

	if (!dev::Video::Open())
	{
		spdlog::error("Video::Open failed!");
		return 1;
	}
	dev::Video::SetVSync(dev::Video::VSync);

	if (!Resources::Load("gamedata.mpack"))
	{
		spdlog::error("Giving up, failed to load game's assets...");
		return false;
	}

	if (!video::Render::Init())
		return false;

	spdlog::info("");
	spdlog::info("Now entering main loop....");

	if (!SetScript("init.lua"))
	{
		spdlog::error("Failed to load init.lua");
		return 1;
	}

	const double TARGET_FPS = (1000.0 / 60);
	uint64_t freq = SDL_GetPerformanceFrequency();
	uint64_t start = 0, end = 0;
	
	uint64_t memUsage = Resources::GetMemUsage();
	double calcMem = 0;
	double delta = 0;

	bool running = true;
	while (running)
	{
		Input::prevKeyStates = Input::keyStates;

		// Check if we have to move to the next script
		if (shouldSwitch)
		{
			script = nextScript; /* Swap */
			tick = (*script)["tick"];
			draw = (*script)["draw"];
			shouldSwitch = false;
		}

		// Calculate pack memory usage every 5 seconds
		calcMem += delta;
		if (calcMem >= 5)
		{
			memUsage = Resources::GetMemUsage();
			calcMem = 0;
		}

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					running = false;
					break;

				case SDL_KEYDOWN:
					Input::keyStates[e.key.keysym.sym] = true;
					Input::prevKeyStates[e.key.keysym.sym] = false;
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

				default:
					spdlog::info("Unhandeled event {}", e.type);
					break;
			}
		}
		
		// Frame start
		start = SDL_GetPerformanceCounter();

		script->PushGlobal(tick);
		script->Push(delta * 60);
		script->Push(delta);
		tick.Call();

		video::Render::Clear(video::Colors::Black);
		video::Render::DrawString(std::format("update {:.2f}ms\nmem {:.1f}mb", delta * 1000.0, memUsage / 1000000.0), 1, 1);

		script->PushGlobal(draw);
		script->Push(delta * 60);
		script->Push(delta);
		draw.Call();

		// Frame end
		dev::Video::Swap();
		end = SDL_GetPerformanceCounter();
		delta = ((1 * (end - start)) / (double)freq);
	}

	return 0;
}

bool MainLoop::SetScript(const std::string name, const std::string& groupName)
{
	shouldSwitch = true;
	nextScript = std::make_shared<lua::Lua>();

	if (!nextScript->Do(name, groupName))
		return false;

	return true;
}