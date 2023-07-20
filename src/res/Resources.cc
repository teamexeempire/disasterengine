#include "Resources.h"
#include <SDL2/SDL_endian.h>
#include <spdlog/spdlog.h>
#include <fstream>

#define MODPACKVER "HPAKv1\0"

bool Resources::Load(const std::string& path, const std::string& groupName)
{
	spdlog::info("Loading modpack \"{}\"...", path);

	std::ifstream file { path, std::ios::binary };
	if (file.fail())
	{
		spdlog::error("Failed to open ModPack file!", path);
		return false;
	}

	char s[7];
	file.read(s, 6);
	s[6] = '\0'; // null not included

	if (strcmp(s, MODPACKVER) != 0)
	{
		spdlog::error("Signature mismatch! ({} required, got {})", MODPACKVER, s);
		return false;
	}

	while (true)
	{
		if (file.peek() == EOF)
			break;

		// Name
		std::string name;
		uint8_t nameSize;
		file.read((char*)&nameSize, 1);

		for (uint8_t i = 0; i < nameSize; i++)
		{
			char c;
			file.read(&c, 1);

			name.append(&c, 1);
		}

		// Type
		ResourceType type;
		file.read((char*)&type, 1);

		// Length
		uint64_t len;
		file.read((char*)&len, 8);

		uint8_t* data = new uint8_t[len];
		file.read((char*)data, len);

		Store(name, groupName, type, data, len);
	}

	file.close();
	return true;
}

bool Resources::Store(const std::string& name, const std::string& groupName, ResourceType type, const uint8_t* data, uint64_t size)
{	
	spdlog::info("> Resource \"{}\" (groupName {}, type {})...", name, groupName, (uint8_t)type);
	if (!store.contains(groupName))
		store[groupName] = {};

	switch (type)
	{
		case ResourceType::ByteArray:
		{
			auto res = std::make_shared<ArrayResource>();
			if (!res->Load(data, size))
				return false;

			store[groupName][name] = res;
			break;
		}

		case ResourceType::Sprite:
		{
			auto res = std::make_shared<SpriteResource>();
			if (!res->Load(data, size))
				return false;

			store[groupName][name] = res;
			break;
		}

		case ResourceType::Sample:
		{
			auto res = std::make_shared<SampleResource>();
			if (!res->Load(data, size))
				return false;

			store[groupName][name] = res;
			break;
		}

		case ResourceType::Music:
		{
			auto res = std::make_shared<MusicResource>();
			if (!res->Load(data, size))
				return false;

			store[groupName][name] = res;
			break;
		}
	}

	return true;
}

bool Resources::Store(const std::string& name, ResourceType type, const uint8_t* data, uint64_t size)
{
	return Store(name, "default", type, data, size);
}

uint64_t Resources::GetMemUsage()
{
	uint64_t total = 0;

	for (auto& it : store)
	{
		for (auto& it2 : it.second)
		{
			total += it2.second->Size();
		}
	}

	return total;
}

void Resources::Release(const std::string& name, const std::string& groupName)
{
	if (!Get<Resource>(name, groupName))
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
