#include "ModPack.h"
#include "Resources.h"
#include <SDL2/SDL_endian.h>
#include <spdlog/spdlog.h>

#define MODPACKVER '1'

// ModPack structure
// 
// +------ HEADER -------+
// [0..6] signature "DMPKvX" (X - revision)
// +---------------------+
// 
// +------ CHUNK --------+
// uint64_t chunkSize (Little-Endian)
// char* chunkName (Null-Terminated)
// ......... (Data)
// ......... (Data)
// +---------------------+
bool ModPack::Load(const std::string& path, const std::string& groupName)
{
	std::ifstream file { path, std::ios::binary };

	if (file.fail())
	{
		spdlog::error("Failed to open ModPack file!", path);
		return false;
	}

	char s[7];
	s[6] = '\0';
	file.read(s, 6);

	if (!(s[0] == 'D' && s[1] == 'M' && s[2] == 'P' && s[3] == 'K' && s[4] == 'v' && s[5] == MODPACKVER))
	{
		spdlog::error("Signature mismatch: {}!", s);
		return false;
	}

	while (true)
	{
		if (file.peek() == EOF)
			break;

		std::string name = "";
		uint8_t* data = nullptr;
		uint64_t size = 0;

		// Size
		file.read((char*)&size, sizeof(uint64_t));
		size = SDL_SwapLE64(size); //swap from Little-Endian

		// Name
		char ch;
		while (true)
		{
			file.read(&ch, sizeof(char));

			if (ch == '\0')
				break;

			name.append(&ch, 1);
		}

		// Data
		data = new uint8_t[size];
		file.read((char*)data, size);

		// Push entry into resources
		Resources::Store(name, groupName, data, size);
	}

	file.close();
	spdlog::info("Loaded modpack \"{}\"", path);
	return true;
}
