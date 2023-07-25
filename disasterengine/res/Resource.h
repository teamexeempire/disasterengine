#pragma once
#include "../video/Render.h"
#include "../audio/Audio.h"

enum class ResourceType : uint8_t
{
	ByteArray = 0U,
	Sprite = 1U,
	Sample = 2U,
	Music = 3U
};

// Container for resource
class Resource
{
public:
	inline virtual bool Load(const uint8_t* data, uint64_t size) { return false; }
	inline virtual const uint64_t Size() { return 0; }
	inline virtual const ResourceType Type() { return ResourceType::ByteArray; }

private:
	friend class Resources;
};

class ArrayResource : public Resource
{
public:
	inline ~ArrayResource() { if(data) delete[] data; }

	inline bool Load(const uint8_t* data, uint64_t size) override
	{
		this->data = data;
		this->size = size;
		return true;
	}

	inline const void* Data() { return (void*)data; }

	inline const uint64_t Size() override { return size; }
	inline const ResourceType Type() override { return ResourceType::ByteArray; }

private:
	const uint8_t* data = nullptr;
	uint64_t size = 0;
};

class SpriteResource : public Resource
{
public:
	inline bool Load(const uint8_t* data, uint64_t size) override
	{
		auto res = spr.Load(data, size);
		if (res)
			loaded = true;

		// In any case, free mem (Texture is loaded in gpu mem)
		delete[] data;
		return res;
	}

	inline const video::Sprite* Data() { return loaded ? &spr : nullptr; }
	inline const uint64_t Size() override { return 0; }
	inline const ResourceType Type() override { return ResourceType::Sprite; }

private:
	bool loaded = false;
	video::Sprite spr;
};

class SampleResource : public Resource
{
public:
	inline bool Load(const uint8_t* data, uint64_t size) override
	{
		this->size = size;
		auto res = smp.Load(data, size);
		if (res)
			loaded = true;

		// In any case, free mem (Mix_Chunk copies the data)
		delete[] data;
		return res;
	}

	inline const audio::Sample* Data() { return loaded ? &smp : nullptr; }
	inline const uint64_t Size() override { return size; }
	inline const ResourceType Type() override { return ResourceType::Sample; }

private:
	bool loaded = false;
	audio::Sample smp;
	uint64_t size = 0;
};

class MusicResource : public Resource
{
public:
	inline ~MusicResource() { 
		if(data) 
			delete[] data; 
	}

	inline bool Load(const uint8_t* data, uint64_t size) override
	{
		auto res = mus.Load(data, size);
		if (!res)
		{
			delete[] data;
			data = nullptr;
		}
		else
			loaded = true;

		this->data = data;
		this->size = size;
		return res;
	}

	inline const audio::Music* Data() { return loaded ? &mus : nullptr; }
	inline const uint64_t Size() override { return size; }
	inline const ResourceType Type() override { return ResourceType::Music; }

private:
	bool loaded = false;
	audio::Music mus;
	const uint8_t* data = nullptr;
	uint64_t size = 0;
};