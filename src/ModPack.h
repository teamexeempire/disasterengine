#pragma once
#include <fstream>
#include <map>

class ModPack
{
public:
	static bool Load(const std::string& path, const std::string& groupName = "default");
};