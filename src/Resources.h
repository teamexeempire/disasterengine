#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>

class Resource
{
public:
	Resource() {}
	Resource(const uint8_t* data, uint64_t size);
	~Resource();

	const uint8_t* Data() const;
	const uint64_t Size() const;

private:
	friend class Resources;

	const uint8_t* data;
	uint64_t size;
};

class Resources
{
public:
	static const Resource* Store(const std::string& name, const std::string& group, const uint8_t* data, uint64_t length);
	static const Resource* Store(const std::string& name, const uint8_t* data, uint64_t length);

	static bool Has(const std::string& name, const std::string& groupName = "default");
	static const Resource* Get(const std::string& name, const std::string& groupName = "default");

	static void Release(const std::string& name, const std::string& groupName = "default");
	static void ReleaseGroup(const std::string& groupName);

private:
	static inline std::map<std::string, std::map<std::string, std::shared_ptr<Resource>>> store;
};