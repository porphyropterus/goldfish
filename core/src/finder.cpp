#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

#include "WindArgParser.h"
#include "lib/RP/RPGolConfig.h"
#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPGolDifficulty.h"

#include "types.h"

#include "util_zlib.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <filename> <wind>" << std::endl;
        return 1;
    }

    // parse wind argument
    RPGolWindSet wind;
    if (!WindArgParser::parseTargetWindSet(argv[2], wind))
    {
        std::cerr << "Error: Invalid wind argument" << std::endl;
        return 1;
    }

    // char buffer[1024];
    // wind.toString(buffer);
    // std::cout << "Parsed wind: " << buffer << std::endl;

    auto file_path = argv[1];
    std::ifstream file(file_path, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    // get possible hashes for this wind set
    std::vector<u32> hashes = wind.hashesWithDepth(3);

    // std::cout << "Hashes: ";
    // for (const auto &h : hashes)
    // {
    //     std::cout << h << " ";
    // }
    // std::cout << std::endl;

    // used later
    RPGolWindSet seedWind;

    for (const auto &hash : hashes)
    {
        // std::cout << "Hash: " << std::to_string(hash) << std::endl;

        if (hash > 1 << 21)
        {
            continue;
        }

        // hash number of u64s + 1 u32 for number of offsets
        file.seekg(hash * sizeof(u64) + sizeof(u32), std::ios::beg);

        // read the offset
        u64 offset;
        file.read(reinterpret_cast<char *>(&offset), sizeof(u64));

        // std::cout << "Offset: " << std::hex << offset << std::endl;

        // read the next offset to determine the size of the data
        u64 next_offset;
        file.read(reinterpret_cast<char *>(&next_offset), sizeof(u64));

        // std::cout << "Next offset: " << next_offset << std::endl;

        // calculate the size of the data
        u64 difference = next_offset - offset;
        if (difference < sizeof(u64))
        {
            continue;
        }
        u64 size = difference - sizeof(u64);

        // std::cout << "Size: " << size << std::endl;

        // seek offset amount of bytes ahead
        file.seekg(offset - (2 * sizeof(u64)), std::ios::cur);

        // uncompress the data
        std::vector<u8> compressedData(size);
        file.read(reinterpret_cast<char *>(compressedData.data()), size);

        // for (size_t i = 0; i < 10; ++i)
        // {
        //     std::cout << std::hex << static_cast<int>(compressedData[i]) << " ";
        // }
        // std::cout << std::endl;

        std::vector<u8> decompressedData;
        decompressData(compressedData, decompressedData);

        // for (size_t i = 0; i < 10; ++i)
        // {
        //     std::cout << std::hex << static_cast<int>(decompressedData[i]) << " ";
        // }
        // std::cout << std::endl;

        // construct seeds array

        // size is the first 4 bytes
        u32 seeds_size = 0;
        for (int i = 0; i < 4; ++i)
        {
            seeds_size |= (decompressedData[i] << (8 * i));
        }

        // std::cout << "Seeds size: " << seeds_size << std::endl;

        // construct the seeds array (the data is stored MSB->LSB)
        std::vector<u32> seeds(seeds_size);

        int read_pos = 4;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < seeds_size; j++)
            {
                seeds[j] |= (decompressedData[read_pos] << (8 * (3 - i)));
                read_pos++;
                if (i == 3 && j < 10)
                {
                    // std::cout << "Seed[" << std::setw(2) << std::setfill('0') << j << "]: 0x"
                    //           << std::hex << std::setw(8) << std::setfill('0') << seeds[j] << std::endl;
                }
            }
        }

        // un-delta encode the seeds
        for (u32 i = 1; i < seeds.size(); i++)
        {
            seeds[i] += seeds[i - 1];
        }

        for (int i = 0; i < seeds.size(); i++)
        {
            RPUtlRandom::GetInstance()->initialize(seeds[i]);
            RPGolConfig::getInstance()->MakeWindSet(diff_Ninehole, seedWind);

            if (wind == seedWind)
            {
                std::cout << "Seed: 0x" << std::setw(8) << std::setfill('0') << std::hex << seeds[i] << std::endl;

                char buffer[1024] = {0};
                seedWind.toString(buffer);
                std::cout << "Wind: " << buffer << "\n\n";
            }
        }
    }
}