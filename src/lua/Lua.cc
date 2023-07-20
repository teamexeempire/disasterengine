#include "Lua.h"
#include "LuaExt.h"
#include "../res/Resources.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace lua
{
	Lua::Lua()
	{
		L = luaL_newstate();
	}

	Lua::~Lua()
	{
		lua_close(L);
	}

	bool Lua::Do(const std::string& name, const std::string& groupName)
	{
		if (!L)
		{
			spdlog::error("Failed to create LUA state!");
			return false;
		}

		auto res = Resources::Get<ArrayResource>(name, groupName);
		if (!res)
		{
			spdlog::error("Failed to load script \"{}\" (group {}): Key doesn't exist.", name, groupName);
			return false;
		}

		lua_pushcfunction(L, luaopen_base);
		lua_pushstring(L, "");
		lua_call(L, 1, 0);

		lua_pushcfunction(L, luaopen_package);
		lua_pushstring(L, LUA_LOADLIBNAME);
		lua_call(L, 1, 0);

		lua_pushcfunction(L, luaopen_package);
		lua_pushstring(L, LUA_TABLIBNAME);
		lua_call(L, 1, 0);

		lua_pushcfunction(L, luaopen_package);
		lua_pushstring(L, LUA_STRLIBNAME);
		lua_call(L, 1, 0);

		lua_pushcfunction(L, luaopen_package);
		lua_pushstring(L, LUA_MATHLIBNAME);
		lua_call(L, 1, 0);

		lua_pushcfunction(L, luaopen_package);
		lua_pushstring(L, LUA_DBLIBNAME);
		lua_call(L, 1, 0);

		RegisterExt(L);

		std::string code;
		code.append((const char*)res->Data(), res->Size());

		if (luaL_dostring(L, code.c_str()))
		{
			spdlog::warn("LUA error: {}", luaL_checkstring(L, -1));
			return false;
		}

		return true;
	}

	bool Lua::Has(const LuaLString& name)
	{
		lua_getglobal(L, name.c_str());
		bool result = lua_isnil(L, -1);

		if (!result)
			lua_pop(L, 1);
		
		return result;
	}

	LuaGlobal Lua::operator[](const LuaLString& name)
	{
		lua_getglobal(L, name.c_str());

		if (lua_isnil(L, -1))
		{
			spdlog::error("LUA global \"{}\" is nil!", name);
			return {};
		}

		LuaGlobal global;
		global.parent = this;
		global.name = name;
		lua_pop(L, 1);

		return global;
	}

	LuaGlobal& Lua::PushGlobal(LuaGlobal& global)
	{
		lua_getglobal(L, global.name.c_str());
		return global;
	}

	void Lua::Push(LuaNumber value)
	{
		lua_pushnumber(L, value);
		stack++;
	}

	void Lua::PushString(LuaString string)
	{
		lua_pushstring(L, string);
		stack++;
	}

	void Lua::PushString(LuaLString string)
	{
		lua_pushlstring(L, string.c_str(), string.length());
		stack++;
	}

	void Lua::Push(LuaBoolean boolean)
	{
		lua_pushboolean(L, boolean);
		stack++;
	}

	void Lua::PushNil()
	{
		lua_pushnil(L);
		stack++;
	}

	void Lua::Pop(int cnt)
	{
		lua_pop(L, cnt);
	}

	LuaLString LuaGlobal::Name() const
	{
		return name;
	}

	LuaNumber LuaGlobal::Number()
	{
		lua_getglobal(parent->L, name.c_str());

		LuaNumber value = lua_tonumber(parent->L, -1);
		lua_pop(parent->L, 1);

		return value;
	}

	LuaLString LuaGlobal::String()
	{
		lua_getglobal(parent->L, name.c_str());

		LuaLString value = lua_tostring(parent->L, -1);
		lua_pop(parent->L, 1);

		return value;
	}

	LuaBoolean LuaGlobal::Bool()
	{
		lua_getglobal(parent->L, name.c_str());

		LuaBoolean value = lua_toboolean(parent->L, -1);
		lua_pop(parent->L, 1);

		return value;
	}

	bool LuaGlobal::Call()
	{
		if (!parent)
			return false;

		if (lua_pcall(parent->L, parent->stack, 0, 0) != LUA_OK)
		{
			spdlog::warn("LUA Runtime error: {}", lua_tostring(parent->L, -1));

			parent->stack = 0;
			return false;
		}

		parent->stack = 0;
		return true;
	}
}
