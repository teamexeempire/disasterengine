#include "config.h"
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <fstream>

#ifdef BIG_ENDIAN

uint16_t _swap16(uint16_t val)
{
    return (val << 8) | (val >> 8);
}

uint32_t _swap32(uint32_t val)
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
    return (val << 16) | (val >> 16);
}

uint64_t _swap64(uint64_t val)
{
    val = ((val << 8) & 0xFF00FF00FF00FF00ULL) | ((val >> 8) & 0x00FF00FF00FF00FFULL);
    val = ((val << 16) & 0xFFFF0000FFFF0000ULL) | ((val >> 16) & 0x0000FFFF0000FFFFULL);
    return (val << 32) | (val >> 32);
}

#endif

uint8_t getType(std::string ext)
{
    if (ext == ".ogg") return 3;
    if (ext == ".wav") return 2;
    if (ext == ".png") return 1;
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        puts
        (
            "modpack v" MODPACKVER " utility for packaging mods\n"
            "(c) 2023 Team Exe Empire\n\n"
            "Usage: modpack [mod folder] [output file]"
        );
    
        return 1;
    }

    std::cout << "Checking IN dir..." << std::endl;
    if (!std::filesystem::exists(argv[1]) || !std::filesystem::is_directory(argv[1]))
    {
        std::cerr << "Invalid path \"" << argv[1] << "\"";
        return 1;
    }

    std::fstream out { argv[2], std::ios::out | std::ios::binary };

    const char* HEADER = "HPAKv1";
    out.write(HEADER, 6);

    for (const auto& entry : std::filesystem::recursive_directory_iterator(argv[1]))
    {
        if (entry.is_directory())
            continue;

        std::fstream file { entry.path(), std::ios::in | std::ios::binary };
        if (file.fail())
        {
            std::cout << "Failed to open " << entry.path() << std::endl;
            return 1;
        }

        file.seekg(0, std::ios::end);
        uint64_t len = file.tellg();
        file.seekg(0, std::ios::beg);

        // Read file
        char* buffer = new char[len];
        memset(buffer, 0, len);
        file.read(buffer, len);
        file.close();

        auto fname = entry.path().filename().string();
        uint8_t fnameLen = fname.length();
        auto ext = entry.path().extension().string();
        uint8_t type = getType(ext);

        out.write((char*)&fnameLen, 1);
        out.write(fname.c_str(), fname.length());
        out.write((char*)&type, 1);
        out.write((char*)&len, 8);
        out.write(buffer, len);
        delete[] buffer;

        std::cout << entry.path().filename() << "..." << std::endl;
    }

    out.close();
    std::cout << "Done." << std::endl;
    return 0;
}