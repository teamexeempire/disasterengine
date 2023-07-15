#include "LuaExt.h"
#include <string.h>

namespace lua
{
#define REGISTER(name, ptr) lua_pushcfunction(state, ptr); lua_setglobal(state, name);

	static void LoadSprite(lua_State* state)
	{
		
	}

	void registerExt(lua_State* state)
	{

	}
}