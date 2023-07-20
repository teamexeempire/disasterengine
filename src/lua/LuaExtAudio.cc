#include "LuaExt.h"
#include "../res/Resources.h"
#include "../audio/Audio.h"
#include <spdlog/spdlog.h>

namespace lua
{
	LUA_FUNC(LoadSample)
	{
		lua::LuaLString resName = luaL_checkstring(L, 1);
		lua::LuaLString resGroup = luaL_checkstring(L, 2);

		auto res = Resources::Get<SampleResource>(resName, resGroup);
		if (!res)
		{
			spdlog::error("Failed to load sample {} (groupName {}): Key doesn't exist!", resName, resGroup);
			lua_pushnil(L);
			return 1;
		}

		auto samp = res->Data();
		if (!samp)
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushlightuserdata(L, (void*)samp);
		return 1;
	}

	LUA_FUNC(LoadMusic)
	{
		lua::LuaLString resName = luaL_checkstring(L, 1);
		lua::LuaLString resGroup = luaL_checkstring(L, 2);

		auto res = Resources::Get<MusicResource>(resName, resGroup);
		if (!res)
		{
			spdlog::error("Failed to load music {} (groupName {}): Key doesn't exist!", resName, resGroup);
			lua_pushnil(L);
			return 1;
		}

		auto mus = res->Data();
		if (!mus)
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushlightuserdata(L, (void*)mus);
		return 1;
	}

	LUA_FUNC(PlaySample)
	{
		if (lua_isnil(L, 1))
		{
			spdlog::error("Failed to play sample: argument is nil!");
			lua_pushnil(L);
			return 1;
		}

		// lets hope that user isnt stupid because thats all we can do
		audio::Sample* samp = (audio::Sample*)lua_touserdata(L, 1);
		lua::LuaBoolean loop = lua_toboolean(L, 2);

		if (!samp)
		{
			spdlog::error("Failed to play sample: argument is nullptr_t!");
			lua_pushnil(L);
			return 1;
		}

		int channel = dev::Audio::PlaySample(*samp, loop);
		lua_pushnumber(L, channel);
		return 1;
	}

	LUA_FUNC(StopSample)
	{
		lua::LuaNumber ch = luaL_checknumber(L, 1);
		dev::Audio::StopChannel((int)ch);
		return 0;
	}

	LUA_FUNC(PlayMusic)
	{
		if (lua_isnil(L, 1))
		{
			spdlog::error("Failed to play music: argument is nil!");
			return 0;
		}

		// lets hope that user isnt stupid because thats all we can do
		audio::Music* mus = (audio::Music*)lua_touserdata(L, 1);
		lua::LuaBoolean loop = lua_toboolean(L, 2);

		if (!mus)
		{
			spdlog::error("Failed to play music: argument is nullptr_t!");
			lua_pushnil(L);
			return 1;
		}

		dev::Audio::PlayMusic(*mus, loop);
		return 0;
	}

	LUA_FUNC(StopMusic)
	{
		dev::Audio::StopMusic();
		return 0;
	}

	LUA_FUNC(SetMusicVolume)
	{
		lua::LuaNumber value = luaL_checknumber(L, 1);
		dev::Audio::SetMusicVolume(value);
		return 0;
	}

	void RegisterAudioExt(lua_State* state)
	{
		// Load
		lua_register(state, "music_load", LoadMusic);
		lua_register(state, "sample_load", LoadSample);

		// Play/Pause
		lua_register(state, "music_play", PlayMusic);
		lua_register(state, "music_volume", SetMusicVolume);
		lua_register(state, "music_stop", StopMusic);
		lua_register(state, "sample_play", PlaySample);
		lua_register(state, "sample_stop", StopSample);
	}
}