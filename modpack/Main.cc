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

    std::ofstream out { argv[2], std::ios::binary };

    const char HEADER[6] = { 'D', 'M', 'P', 'K', 'v', MODPACKVER[0] };
    const char NULLTERM = '\0';

    out.write(HEADER, 6);

    for (const auto& entry : std::filesystem::recursive_directory_iterator(argv[1]))
    {
        if (entry.is_directory())
            continue;

        std::ifstream file { entry.path(), std::ios::binary };

        if (file.fail())
        {
            std::cout << "retard error" << std::endl;
            return 1;
        }

        file.seekg(0, std::ios::end);
    
        // writing size
        uint64_t size = file.tellg();
        uint64_t _swp = swap64(size);

        out.write((const char*)&_swp, sizeof(size));

        // writing path
        auto path = entry.path().string();
        path = path.substr(path.find_last_of("/\\") + 1);

        out.write(path.c_str(), path.length());
        out.write((const char*)&NULLTERM, 1);

        //now write data
        file.seekg(0, std::ios::beg);

        uint64_t pos = 0;
        while (pos < size)
        {
            char buffer[1024];
            file.read(buffer, 1024);

            uint64_t read = file.gcount();
            out.write(buffer, read);

            pos += read;
        }

        file.close();
        std::cout << entry.path() << std::endl;
    }

    out.close();
    std::cout << "Ok!" << std::endl;
    return 0;
}