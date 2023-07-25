#include "LuaExt.h"
#include "../res/Resources.h"
#include "../video/Video.h"
#include "../video/Render.h"
#include <spdlog/spdlog.h>

namespace lua
{
	LUA_FUNC(LoadSprite)
	{
		lua::LuaLString resName = luaL_checkstring(L, 1);
		lua::LuaLString resGroup = luaL_checkstring(L, 2);

		auto res = Resources::Get<SpriteResource>(resName, resGroup);
		if (!res)
		{
			spdlog::error("Failed to load sprite {} (groupName {}): Key doesn't exist!", resName, resGroup);
			lua_pushnil(L);
			return 1;
		}

		auto spr = res->Data();
		if (!spr)
		{
			lua_pushnil(L);
			return 1;
		}

		lua_pushlightuserdata(L, (void*)spr);
		return 1;
	}

	LUA_FUNC(DrawSprite)
	{
		if (lua_isnil(L, 1))
		{
			spdlog::error("Failed to draw sprite: argument is nil!");
			lua_pushnil(L);
			return 1;
		}

		video::Sprite* spr = (video::Sprite*)lua_touserdata(L, 1);
		lua::LuaNumber x = luaL_checknumber(L, 2);
		lua::LuaNumber y = luaL_checknumber(L, 3);

		if (!spr)
			return 0;

		video::Render::Draw(*spr, x, y);
		return 0;
	}

	LUA_FUNC(DrawSpriteExt)
	{
		if (lua_isnil(L, 1))
		{
			spdlog::error("Failed to draw sprite: argument is nil!");
			lua_pushnil(L);
			return 1;
		}

		video::Sprite* spr = (video::Sprite*)lua_touserdata(L, 1);
		lua::LuaNumber x = luaL_checknumber(L, 2);
		lua::LuaNumber y = luaL_checknumber(L, 3);
		lua::LuaNumber angle = luaL_checknumber(L, 4);
		lua::LuaNumber r = luaL_checknumber(L, 5);
		lua::LuaNumber g = luaL_checknumber(L, 6);
		lua::LuaNumber b = luaL_checknumber(L, 7);
		lua::LuaNumber a = luaL_checknumber(L, 8);

		if (!spr)
			return 0;

		video::Render::Draw(*spr, x, y, angle, video::Color { (int)r, (int)g, (int)b, (int)a });
		return 0;
	}

	LUA_FUNC(DrawString)
	{
		lua::LuaLString string = luaL_checkstring(L, 1);
		lua::LuaNumber x = luaL_checknumber(L, 2);
		lua::LuaNumber y = luaL_checknumber(L, 3);

		video::Render::DrawString(string, x, y);
		return 0;
	}

	LUA_FUNC(VideoSetVSync)
	{
		lua::LuaNumber mode = luaL_checknumber(L, 1);
		dev::Video::SetVSync((dev::Video::VSyncMode)mode);
		return 0;
	}

	void RegisterVideoExt(lua_State* state)
	{
		// VSync
		lua_regconstant(state, "VSYNC_ADAPTIVE", dev::Video::VSyncAdaptive);
		lua_regconstant(state, "VSYNC_ON", dev::Video::VSync);
		lua_regconstant(state, "VSYNC_OFF", dev::Video::NoVSync);
		lua_register(state, "vsync_set", VideoSetVSync);

		lua_register(state, "sprite_load", LoadSprite);
		lua_register(state, "sprite_draw", DrawSprite);
		lua_register(state, "sprite_drawex", DrawSpriteExt);
		lua_register(state, "text_draw", DrawString);
	}
}