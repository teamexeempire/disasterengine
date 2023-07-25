#pragma once
#include "Resource.h"
#include <map>
#include <vector>
#include <string>
#include <memory>

class Resources
{
public:
	static bool Load(const std::string& path, const std::string& groupName = "default");
	static bool Store(const std::string& name, const std::string& group, ResourceType type, const uint8_t* data, uint64_t length);
	static bool Store(const std::string& name, ResourceType type, const uint8_t* data, uint64_t length);
	static uint64_t GetMemUsage();
	
	template<typename T>
	inline static T* Get(const std::string& name, const std::string& groupName = "default")
	{
		if (!store.contains(groupName))
			return nullptr;

		if (!store[groupName].contains(name))
			return nullptr;

		return dynamic_cast<T*>(store[groupName][name].get());
	}


	static void Release(const std::string& name, const std::string& groupName = "default");
	static void ReleaseGroup(const std::string& groupName);

private:
	static inline std::map<std::string, std::map<std::string, std::shared_ptr<Resource>>> store;

};