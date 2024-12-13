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

#include "lib/SIMDCompressionAndIntersection/include/intersection.h"
#include "lib/SIMDCompressionAndIntersection/include/codecfactory.h"

using namespace SIMDCompressionLib;

/// <summary>
/// Finds Dolphin-usable seed with the closest wind set to the target.
/// </summary>
/// <param name="target">Target (ideal) wind set</param>
/// <param name="out">Calendar time output</param>
/// <returns>True if the target was found, otherwise false</returns>
bool tryToFindSeedFromWindSet(const RPGolWindSet &target, OSCalendarTime &out)
{
    //// Highest wind set score so far
    // u32 bestScore = 0;
    //// Best wind set found so far
    // RPGolWindSet bestWind;

    // for (u32 i = 0; i < 0x10000; i++)
    //{
    //     // Init seed
    //     RPUtlRandom::setSeed(i);
    //     // Generate new wind set
    //     RPGolWindSet newWind = RPGolConfig::getInstance()->getWindSet();

    //    // If the newest wind set is closer to the target than the
    //    // best wind set, the new wind set becomes the best wind set
    //    u32 score = newWind.scoreAgainstTarget(target);
    //    if (score > bestScore)
    //    {
    //        bestScore = score;
    //        bestWind = newWind;
    //    }

    //    // If it is a perfect match, we don't need to search anymore
    //    if (score == perfectScore)
    //    {
    //        std::printf("Match found !\n");
    //        return true;
    //    }
    //}

    //// Nested loop to try all possible Dolphin RTC seeds
    // for (u32 i = 0; i < 60; i++) // min
    //{
    //     for (u32 j = 0; j < 60; j++) // sec
    //     {
    //         // Setup calendar time for seed
    //         out.min = i;
    //         out.sec = j;

    //        // Dolphin RTC has a precision of seconds due to it's usage of time_t.
    //        // As a result, when recording or playing back a movie, the millisecond and microsecond values will be set to zero.
    //        // If no movie is being recorded/played, the values are initialized to that of your computer's system time.
    //        // Because this tool is designed for TASing, the values will be initialized to zero here.
    //        out.msec = 0;
    //        out.usec = 0;

    //        // Init seed
    //        RPUtlRandom::initialize(out);
    //        // Generate new wind set
    //        RPGolWindSet newWind = RPGolConfig::getInstance()->getWindSet();

    //        // If the newest wind set is closer to the target than the
    //        // best wind set, the new wind set becomes the best wind set
    //        u32 score = newWind.scoreAgainstTarget(target);
    //        if (score > bestScore)
    //        {
    //            bestScore = score;
    //            bestWind = newWind;
    //        }

    //        // If it is a perfect match, we don't need to search anymore
    //        if (score == perfectScore)
    //        {
    //            std::printf("Match found !\n");
    //            return true;
    //        }
    //    }
    //}

    // std::printf("A seed for %s could not be found.\n The closest seed is %s, which gave the wind set\n%s.",
    //     target.toString().c_str(), OSCalendarTimeToDolphinRTC(out).c_str(), bestWind.toString().c_str());

    return false;
}

void getWindSetFromSeed(u32 seed, RPGolWindSet &out)
{
    RPUtlRandom::initialize(seed);
    RPGolConfig *pInstance = RPGolConfig::getInstance();
    pInstance->MakeWindSet(diff_Ninehole, out);
}

void createFolders(std::filesystem::path path, unsigned char depth, unsigned char speeds, unsigned char directions)
{
    for (u32 i = 0; i < speeds; i++)
    {
        std::filesystem::path inner_path = path / std::to_string(i);

        // make sure we dont generate folders twice
        if (std::filesystem::exists(inner_path))
            return;

        std::filesystem::create_directory(inner_path);
        bool last = depth == 1;
        for (u32 j = 0; j < directions; j++)
        {
            if (last)
            {
                std::ofstream file(inner_path / (std::to_string(j) + ".bin"));
            }
            else
            {
                std::filesystem::path inner_inner_path = inner_path / std::to_string(j);
                std::filesystem::create_directory(inner_inner_path);
                createFolders(inner_inner_path, depth - 1, speeds - 1, directions - 1);
            }
        }
    }
}

void dumpWindSeedPrefixes(std::filesystem::path path, unsigned char depth = 2)
{
    RPGolConfig *pInstance = RPGolConfig::getInstance();
    RPGolWindSet wind;

    createFolders(path, depth, RPGolDefine::MAX_WIND_SPD, RPGolDefine::MAX_WIND_DIR);

    // for all seeds
    for (u32 i = 0x123456; i < 0x123456 + 1000000; i++)
    {
        // get that seed's wind
        getWindSetFromSeed(i, wind);

        // get the right path

        std::filesystem::path write_path = ".\\seeds";

        for (u32 j = 0; j < depth; j++)
        {
            u32 speed = wind.mWinds[j].mSpeed;
            u32 direction = wind.mWinds[j].mDirection;
            write_path /= std::to_string(speed);
            write_path /= std::to_string(direction);
        }

        write_path += ".bin";

        // append to this file
        std::ofstream file(write_path, std::ios::app | std::ios::binary);
        file.write(reinterpret_cast<const char *>(&i), sizeof(u32));
    }
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
        u32 hash = wind.hashWithDepth(1);
        hashToSeeds[hash].seeds.push_back(seed);
    }

public:
    Generator()
    {
        // initialize hashmap
        for (u32 i = 0; i < 1 << 7; i++)
        {
            hashToSeeds[i] = FileAndSeeds();
            hashToSeeds[i].file.open(".\\temp\\" + std::to_string(i) + ".bin", std::ios::binary);
        }
    }

    void dumpSeeds()
    {
        // create folder called temp
        std::filesystem::create_directory(".\\temp");

        u32 seed = 0x12340000;

        for (u32 i = 0; i < 16; i++)
        {
            do
            {
                addSeedToHashMap(seed);
            } while (++seed & 0xFFFF);

            for (auto &pair : hashToSeeds)
            {
                if (!pair.second.seeds.empty())
                {
                    pair.second.file.write(reinterpret_cast<const char *>(pair.second.seeds.data()), pair.second.seeds.size() * sizeof(u32));
                    pair.second.seeds.clear();
                }
            }
            std::cout << i << "/16" << std::endl;
        }
    }
};

int main()
{
    // load temp/0.bin as a vector
    std::ifstream file(".\\temp\\0.bin", std::ios::binary);

    // Generator generator;
    // auto start = std::chrono::high_resolution_clock::now();
    // generator.dumpSeeds();
    // auto end = std::chrono::high_resolution_clock::now();
    // std::chrono::duration<double> elapsed = end - start;
    // std::cout << "Time per seed: " << elapsed.count() / 0x100000 << "s" << std::endl;
    // std::cout << "Time to dump 2^32 seeds: " << (elapsed.count()) * 298.261618 << "h" << std::endl;

    // // open file as an int pointer and start reading it
    // std::ifstream file(".\\seeds\\0\\1.bin", std::ios::binary);
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