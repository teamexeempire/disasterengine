#include "Resources.h"
#include <spdlog/spdlog.h>

Resource::~Resource()
{
	delete[] data;
}

const uint8_t* Resource::Data() const
{
	return data;
}

const uint64_t Resource::Size() const
{
	return size;
}

Resource::Resource(const uint8_t* data, uint64_t size)
{
	this->data = data;
	this->size = size;
}

const Resource* Resources::Store(const std::string& name, const std::string& group, const uint8_t* data, uint64_t size)
{	
	if (!store.contains(group))
		store[group] = {};

	store[group][name] = std::make_shared<Resource>(data, size);
	return store[group][name].get();
}

const Resource* Resources::Store(const std::string& name, const uint8_t* data, uint64_t size)
{
	return Store(name, "default", data, size);
}

bool Resources::Has(const std::string& name, const std::string& groupName)
{
	return Get(name, groupName) != nullptr;
}

const Resource* Resources::Get(const std::string& name, const std::string& groupName)
{
	if (!store.contains(groupName))
		return nullptr;

	if (!store[groupName].contains(name))
		return nullptr;

	return store[groupName][name].get();
}

void Resources::Release(const std::string& name, const std::string& groupName)
{
	if (!Has(name, groupName))
	{
		spdlog::error("Failed to release resource \"{}\" (group {}): Already released.", name, groupName);
		return;
	}

	store[groupName].erase(name);
	spdlog::info("\"{}\" (group {}) released", name, groupName);
}

void Resources::ReleaseGroup(const std::string& groupName)
{
	if (!store.contains(groupName))
	{
		spdlog::error("Failed to release group \"{}\": Doesn't exist.", groupName);
		return;
	}

	store.erase(groupName);
}
