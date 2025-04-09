#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>

#include <vector>
#include <unordered_map>

#include "zlib.h"

#include "types.h"
#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPGolConfig.h"
#include "lib/RP/RPGolWindSet.h"
#include "lib/RP/RPGolDifficulty.h"
#include "lib/rvl/OSTime.h"

void compressData(const std::vector<u8> &data, std::vector<u8> &compressedData)
{
    uLongf compressedSize = compressBound(data.size());
    compressedData.resize(compressedSize);

    if (compress(compressedData.data(), &compressedSize, data.data(), data.size()) != Z_OK)
    {
        throw std::runtime_error("Compression failed");
    }

    compressedData.resize(compressedSize);
}

void getWindSetFromSeed(u32 seed, RPGolWindSet &out)
{
    RPUtlRandom::initialize(seed);
    RPGolConfig *pInstance = RPGolConfig::getInstance();
    pInstance->MakeWindSet(diff_Ninehole, out);
}

class Generator
{
    class FileAndSeeds
    {
    public:
        std::ofstream file;
        std::vector<u32> seeds;
    };

    RPGolConfig *pInstance = RPGolConfig::getInstance();
    RPGolWindSet wind;

    std::unordered_map<u32, FileAndSeeds> hashToSeeds;

    void generateWindSet(u32 seed)
    {
        RPUtlRandom::initialize(seed);
        pInstance->MakeWindSet(diff_Ninehole, wind);
    }

    void addSeedToHashMap(u32 seed)
    {
        generateWindSet(seed);
        std::vector<u32> hash = wind.hashesWithDepth(2);
        hashToSeeds[hash[0]].seeds.push_back(seed);
    }

public:
    Generator()
    {
        // // initialize hashmap
        // for (u32 i = 0; i < 1 << 14; i++)
        // {
        //     hashToSeeds[i] = FileAndSeeds();
        //     hashToSeeds[i].file.open("./temp/" + std::to_string(i) + ".bin", std::ios::binary | std::ios::trunc);
        //     if (!hashToSeeds[i].file)
        //     {
        //         std::cerr << "Error: Could not open file " << i << ".bin" << std::endl;
        //         exit(1);
        //     }
        // }
    }

    ~Generator()
    {
        // for (u32 i = 0; i < 1 << 14; i++)
        // {
        //     hashToSeeds[i].file.close();
        // }
    }

    void dumpSeeds()
    {
        // // create folder called temp
        // std::filesystem::create_directory("./temp");

        // u32 seed = 0x00000000;

        // for (u32 i = 0; i < 0x10000; i++)
        // {
        //     do
        //     {
        //         addSeedToHashMap(seed);
        //     } while (++seed & 0xFFFF);

        //     for (u32 j = 0; j < 1 << 14; j++)
        //     {
        //         if (!hashToSeeds[j].seeds.empty())
        //         {
        //             hashToSeeds[j].file.write(reinterpret_cast<const char *>(hashToSeeds[j].seeds.data()), hashToSeeds[j].seeds.size() * sizeof(u32));
        //             hashToSeeds[j].seeds.clear();
        //         }
        //     }

        //     std::cout << "0x" << std::hex << i << "/0x4000" << std::endl;
        // }

        // for all files in the temp folder
        // rewrite them so the most significant bits are first, then the 2nd most, etc.

        std::vector<u32> seeds;
        std::vector<u8> new_bytes;
        u32 read_seed;

        std::ofstream output_file("./seeds.bin", std::ios::binary | std::ios::trunc);

        // stores number of bytes forward you would need to go to get the corresponding hash
        // fill with 0s for now
        std::vector<u64> offsets(1 << 14, 0);
        // except for the first offset would be 1 << 14 * sizeof(u64)
        offsets[0] = (1 << 14) * sizeof(u64);

        // write the number of offsets to the file

        u32 numOffsets = 1 << 14;
        output_file.write(reinterpret_cast<const char *>(&numOffsets), sizeof(u32));

        // write placeholder offsets to the file
        for (u32 i = 0; i < 1 << 14; i++)
        {
            output_file.write(reinterpret_cast<const char *>(&offsets[i]), sizeof(u64));
        }

        int zlib_counter = 0;

        for (/*int i=0; i < 1<<14;*/ int i = 0; i < 1 << 14; i++)
        {

            seeds.clear();
            new_bytes.clear();

            // read from ith file, put contents into seeds
            std::ifstream file = std::ifstream("./temp/" + std::to_string(i) + ".bin", std::ios::binary);

            if (!file)
            {
                std::cout << "Error: Could not open file " << std::to_string(i) << ".bin" << std::endl;
            }
            else
            {
                while (file.read(reinterpret_cast<char *>(&read_seed), sizeof(u32)))
                {
                    seeds.push_back(read_seed);
                }
            }

            // DELTA ENCODING -> MSB->LSB -> GZIP: WORSE THAN JUST MSB->LSB -> GZIP

            // delta encode the seeds
            if (seeds.size() > 1)
            {
                for (u32 i = seeds.size() - 1; i > 0; i--)
                {
                    seeds[i] -= seeds[i - 1];
                }
            }

            // add size to beginning of the new bytes
            u32 size = seeds.size();
            for (int i = 0; i < 4; ++i)
            {
                new_bytes.push_back((size >> (8 * i)) & 0xFF);
            }

            // convert the seeds to msb -> lsb
            for (u32 byte_index = 0; byte_index < 4; ++byte_index)
            {
                for (const auto &seed : seeds)
                {
                    u8 byte = (seed >> (8 * (3 - byte_index))) & 0xFF;
                    new_bytes.push_back(byte);
                }
            }

            // close input file
            file.close();
            // i can delete the temp file now
            std::filesystem::remove("./temp/" + std::to_string(i) + ".bin");

            // compress the new bytes
            std::vector<u8> compressedData;
            compressData(new_bytes, compressedData);
            // write the compressed data to the file
            output_file.write(reinterpret_cast<const char *>(compressedData.data()), compressedData.size());

            // determine next offset
            if (i != (1 << 14) - 1)
                offsets[i + 1] = offsets[i] + compressedData.size() - sizeof(u64);

            if (i % 0x100 == 0)
            {
                zlib_counter++;
                std::cout << "0x" << std::hex << zlib_counter << "/0x2000" << std::endl;
            }
        }

        // write actual offsets to the file
        output_file.seekp(sizeof(u32), std::ios::beg);
        output_file.write(reinterpret_cast<const char *>(offsets.data()), offsets.size() * (sizeof(u64)));
    }
};

int main()
{
    Generator generator;
    auto start = std::chrono::high_resolution_clock::now();
    generator.dumpSeeds();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time per seed: " << elapsed.count() / (0x800 * 65536) << "s" << std::endl;

    // RPGolWindSet wind;

    // RPGolConfig *pInstance = RPGolConfig::getInstance();
    // pInstance->MakeWindSet(diff_Ninehole, wind);

    // char buf[256];
    // wind.toString(buf, "{", "}", "[", "]", true);
    // std::cout << buf << std::endl;

    // // open file as an int pointer and start reading it
    // std::ifstream file("./seeds/0/1.bin", std::ios::binary);
    // u32 seed;
    // int count;
    // RPGolWindSet wind;
    // auto start = std::chrono::high_resolution_clock::now();
    // while (file.read(reinterpret_cast<char *>(&seed), sizeof(u32)))
    // {
    //     count++;
    //     std::cout << "Seed: " << seed << std::endl;
    //     getWindSetFromSeed(seed, wind);
    // }
    // auto end = std::chrono::high_resolution_clock::now();

    // std::cout << "Time per seed: " << std::chrono::duration<double>(end - start).count() / count << "s" << std::endl;
    // std::cout << "Total time: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;

    return 0;
}