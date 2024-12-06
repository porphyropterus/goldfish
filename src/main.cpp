#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "types.h"
#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPGolConfig.h"
#include "lib/RP/RPGolWindSet.h"
#include "lib/RP/RPGolDifficulty.h"
#include "lib/rvl/OSTime.h"

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
    // example: in og, we would create files like 15/7/14/6/13/5.bin

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

void dumpWindSeedPrefixes(std::filesystem::path path, unsigned char depth = 1)
{
    RPGolConfig *pInstance = RPGolConfig::getInstance();
    RPGolWindSet wind;

    createFolders(path, depth, RPGolDefine::MAX_WIND_SPD, RPGolDefine::MAX_WIND_DIR);

    // for all seeds
    for (u32 i = 0x123456; i < 0x123456 + 10; i++)
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

        std::cout << write_path << std::endl;

        // append to this file
        std::ofstream file(write_path, std::ios::app | std::ios::binary);
        file.write(reinterpret_cast<const char *>(&i), sizeof(u32));
    }
}

int main()
{
    dumpWindSeedPrefixes(".\\seeds");
    return 0;
}