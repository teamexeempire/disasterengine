#pragma once
#include <lua.hpp>

namespace lua
{
	class Lua;
	void registerExt(lua_State* state);
}