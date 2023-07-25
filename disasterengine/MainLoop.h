#pragma once
#include "lua/Lua.h"
#include <memory>

class MainLoop
{
public:
	static int Run();
	static bool SetScript(const std::string path, const std::string& groupName = "default");

private:
	static inline std::shared_ptr<lua::Lua> script;
	static inline lua::LuaGlobal tick, draw;

	// Scene stuff
	static inline bool shouldSwitch;
	static inline std::shared_ptr<lua::Lua> nextScript;
};