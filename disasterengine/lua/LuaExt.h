#pragma once
#include "Lua.h"

namespace lua
{
#define LUA_FUNC(x) int x(lua_State* L)
#define lua_regconstant(L, name, x) lua_pushnumber(L, x); lua_setglobal(L, name);

	class Lua;
	void RegisterExt(lua_State* state);
	void RegisterVideoExt(lua_State* state);
	void RegisterAudioExt(lua_State* state);
	void RegisterCoreExt(lua_State* state);
	void RegisterMathExt(lua_State* state);
}