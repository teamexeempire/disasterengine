#include "LuaExt.h"
#include "Lua.h"
#include "../Input.h"
#include <string>

namespace lua
{
	void RegisterExt(lua_State* state)
	{
		RegisterCoreExt(state);
		RegisterMathExt(state);
		RegisterVideoExt(state);
		RegisterAudioExt(state);
	}
}