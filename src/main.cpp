#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>

#include <vector>
#include <unordered_map>

#include "types.h"
#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPGolConfig.h"
#include "lib/RP/RPGolWindSet.h"
#include "lib/RP/RPGolDifficulty.h"
#include "lib/rvl/OSTime.h"

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
        u32 hash = wind.hashWithDepth(2);
        hashToSeeds[hash].seeds.push_back(seed);
    }

public:
    Generator()
    {
        // initialize hashmap
        for (u32 i = 0; i < 1 << 14; i++)
        {
            hashToSeeds[i] = FileAndSeeds();
            hashToSeeds[i].file.open("./temp/" + std::to_string(i) + ".bin", std::ios::binary);
        }
    }

    ~Generator()
    {
        for (u32 i = 0; i < 1 << 14; i++)
        {
            hashToSeeds[i].file.close();
        }
    }

    void dumpSeeds()
    {
        // create folder called temp
        std::filesystem::create_directory("./temp");

        u32 seed = 0x00000000;

        for (u32 i = 0; i < 0x4000; i++)
        {
            do
            {
                addSeedToHashMap(seed);
            } while (++seed & 0xFFFF);

            for (u32 j = 0; j < 1 << 14; j++)
            {
                if (!hashToSeeds[j].seeds.empty())
                {
                    hashToSeeds[j].file.write(reinterpret_cast<const char *>(hashToSeeds[j].seeds.data()), hashToSeeds[j].seeds.size() * sizeof(u32));
                    hashToSeeds[j].seeds.clear();
                }
            }

            std::cout << "0x" << std::hex << i << "/0x4000" << std::endl;
        }

        // for all files in the temp folder
        // rewrite them so the most significant bits are first, then the 2nd most, etc.

        std::vector<u32> seeds;
        std::vector<u8> new_bytes;
        u32 read_seed;

        for (const auto &entry : std::filesystem::directory_iterator("./temp"))
        {
            seeds.clear();
            new_bytes.clear();
            std::ifstream file(entry.path(), std::ios::binary);
            while (file.read(reinterpret_cast<char *>(&read_seed), sizeof(u32)))
            {
                seeds.push_back(read_seed);
            }

            // DELTA ENCODING -> MSB->LSB -> GZIP: WORSE THAN JUST MSB->LSB -> GZIP
            for (u32 i = 1; i < seeds.size(); i++)
            {
                seeds[i] -= seeds[i - 1] - 1;
            }

            for (u32 byte_index = 0; byte_index < 4; ++byte_index)
            {
                for (const auto &seed : seeds)
                {
                    u8 byte = (seed >> (8 * (3 - byte_index))) & 0xFF;
                    new_bytes.push_back(byte);
                }
            }

            std::ofstream new_file(entry.path(), std::ios::binary | std::ios::trunc);
            new_file.write(reinterpret_cast<const char *>(new_bytes.data()), new_bytes.size());
        }
    }
};

int main()
{
    Generator generator;
    auto start = std::chrono::high_resolution_clock::now();
    generator.dumpSeeds();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time per seed: " << elapsed.count() / (4096 * 65536) << "s" << std::endl;
    std::cout << "Time to dump 2^32 seeds: " << (elapsed.count()) * 298.261618 << "h" << std::endl;

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