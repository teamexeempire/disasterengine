#pragma once
#include <lua.hpp>
#include <string>

namespace lua
{
	using LuaNumber  = double;
	using LuaString = char*;
	using LuaLString = std::string;
	using LuaBoolean = bool;

	class Lua;
	class LuaGlobal
	{
	public:
		LuaLString Name() const;

		LuaNumber  Number();
		LuaLString String();
		LuaBoolean Bool();

		bool Call();

	private:
		friend class Lua;
		LuaLString name;
		Lua* parent = nullptr;
	};

	class Lua
	{
	public:
		Lua();
		~Lua();

		bool Do(const std::string& name, const std::string& groupName = "default");

		void Push(LuaNumber value);
		void Push(LuaBoolean boolean);
		void PushNil();
		void PushString(LuaString string);
		void PushString(LuaLString string);
		LuaGlobal& PushGlobal(LuaGlobal& global);

		void Pop(int cnt);

		bool Has(const LuaLString& name);
		LuaGlobal operator[](const LuaLString& name);

	private:
		friend class LuaGlobal;
		lua_State* L = nullptr;

		int stack = 0;
	};
}