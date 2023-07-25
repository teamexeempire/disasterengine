#include "LuaExt.h"
#include "../Input.h"
#include "../res/Resources.h"
#include "../MainLoop.h"

namespace lua
{
	LUA_FUNC(LoadModule)
	{
		lua::LuaLString name = luaL_checkstring(L, 1);
		lua::LuaLString groupName = luaL_checkstring(L, 2);

		auto res = Resources::Get<ArrayResource>(name, groupName);
		if (!res)
		{
			lua_pushnil(L);
			return 1;
		}

		std::string data;
		data.append((const char*)res->Data(), res->Size());

		luaL_dostring(L, data.c_str());
		return 1;
	}

	LUA_FUNC(SetScript)
	{
		lua::LuaLString name = luaL_checkstring(L, 1);
		lua::LuaLString groupName = luaL_checkstring(L, 2);

		MainLoop::SetScript(name, groupName);
		return 0;
	}

	LUA_FUNC(Ord)
	{
		lua::LuaLString string = luaL_checkstring(L, 1);
		lua::LuaNumber ord = (int)string[0];

		lua_pushnumber(L, ord);
		return 1;
	}

	LUA_FUNC(KeyPressed)
	{
		lua::LuaNumber key = luaL_checknumber(L, 1);
		lua_pushboolean(L, Input::IsKeyPressed((int)key));
		return 1;
	}

	LUA_FUNC(KeyDown)
	{
		lua::LuaNumber key = luaL_checknumber(L, 1);
		lua_pushboolean(L, Input::IsKeyDown((int)key));
		return 1;
	}

	LUA_FUNC(KeyUp)
	{
		lua::LuaNumber key = luaL_checknumber(L, 1);
		lua_pushboolean(L, Input::IsKeyUp((int)key));
		return 1;
	}

	void RegisterCoreExt(lua_State* state)
	{
		// Constants
		lua_regconstant(state, "KEY_LEFT", Input::Left);
		lua_regconstant(state, "KEY_RIGHT", Input::Right);
		lua_regconstant(state, "KEY_UP", Input::Up);
		lua_regconstant(state, "KEY_DOWN", Input::Down);
		lua_regconstant(state, "KEY_SPACE", Input::Space);
		lua_regconstant(state, "KEY_CTRL", Input::Ctrl);
		lua_regconstant(state, "KEY_LCTRL", Input::LCtrl);
		lua_regconstant(state, "KEY_RCTRL", Input::RCtrl);
		lua_regconstant(state, "KEY_ALT", Input::Alt);
		lua_regconstant(state, "KEY_LALT", Input::LAlt);
		lua_regconstant(state, "KEY_RALT", Input::RAlt);
		lua_regconstant(state, "KEY_SHIFT", Input::Alt);
		lua_regconstant(state, "KEY_LSHIFT", Input::LShift);
		lua_regconstant(state, "KEY_RSHIFT", Input::RShift);
		lua_regconstant(state, "KEY_TAB", Input::Tab);
		lua_regconstant(state, "KEY_CAPS", Input::Caps);

		// Tools
		lua_register(state, "set_script", SetScript);
		lua_register(state, "module_load", LoadModule);

		// Input
		lua_register(state, "ord", Ord);
		lua_register(state, "keyboard_down", KeyDown);
		lua_register(state, "keyboard_up", KeyUp);
		lua_register(state, "keyboard_pressed", KeyPressed);
	}
}