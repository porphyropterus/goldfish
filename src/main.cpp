#include <sstream>
#include <fstream>
#include <iostream>
#include "types.h"
#include "WindArgParser.h"
#include <RP/RPUtlRandom.h>
#include <RP/RPGolConfig.h>
#include <RP/RPGolWindSet.h>
#include <RP/RPGolDifficulty.h>
#include <rvl/OSTime.h>

bool tryToFindSeedFromWindSet(const RPGolWindSet& target, OSCalendarTime& out);
void getWindSetFromSeed(u32 seed, RPGolWindSet& out);
void dumpWindSeeds(bool bNonRtc);

//int main()
//{
//    //RPGolWindSet set;
//    //OSCalendarTime ctime;
//
//    //set.mWinds[0].mDirection = RPGolDefine::NORTHWEST;
//    //set.mWinds[0].mSpeed = 1;
//
//    //set.mWinds[1].mDirection = RPGolDefine::WILDCARD_DIR;
//    //set.mWinds[1].mSpeed = RPGolDefine::WILDCARD_SPD;
//
//    //set.mWinds[2].mDirection = RPGolDefine::WILDCARD_DIR;
//    //set.mWinds[2].mSpeed = RPGolDefine::WILDCARD_SPD;
//
//    //set.mWinds[3].mDirection = RPGolDefine::WILDCARD_DIR;
//    //set.mWinds[3].mSpeed = RPGolDefine::WILDCARD_SPD;
//
//    //set.mWinds[4].mDirection = RPGolDefine::WILDCARD_DIR;
//    //set.mWinds[4].mSpeed = RPGolDefine::WILDCARD_SPD;
//
//    //set.mWinds[5].mDirection = RPGolDefine::WILDCARD_DIR;
//    //set.mWinds[5].mSpeed = RPGolDefine::WILDCARD_SPD;
//
//    //set.mWinds[6].mDirection = RPGolDefine::WILDCARD_DIR;
//    //set.mWinds[6].mSpeed = RPGolDefine::WILDCARD_SPD;
//
//    //set.mWinds[7].mDirection = RPGolDefine::WILDCARD_DIR;
//    //set.mWinds[7].mSpeed = RPGolDefine::WILDCARD_SPD;
//
//    //set.mWinds[8].mDirection = RPGolDefine::WILDCARD_DIR;
//    //set.mWinds[8].mSpeed = RPGolDefine::WILDCARD_SPD;
//
//    //tryToFindSeedFromWindSet(set, ctime);
//
//    return 0;
//}

int main2(u32 argc, char** argv)
{
    //RPGolWindSet targetWind;
    //OSCalendarTime ctime;

    //// 9 hole default difficulty
    //RPGolDifficulty d = diff_Ninehole;

    //for (u32 i = 0; i < argc; i++)
    //{
    //    // Parse difficulty
    //    if (_stricmp(argv[i], "-d") == 0)
    //    {
    //        switch (*argv[++i])
    //        {
    //        case 'B':
    //            d = diff_Beginner;
    //            break;
    //        case 'I':
    //            d = diff_Intermediate;
    //            break;
    //        case 'E':
    //            d = diff_Expert;
    //            break;
    //        case 'N':
    //            d = diff_Ninehole;
    //            break;
    //        }
    //    }

    //    // Set chosen difficulty
    //    RPGolConfig::getInstance()->setDifficulty(d);

    //    // Find wind set seed
    //    if (_stricmp(argv[i], "-w") == 0)
    //    {
    //        WindArgParser::parseTargetWindSet(std::string(argv[++i]), targetWind);
    //        if (tryToFindSeedFromWindSet(targetWind, ctime))
    //        {
    //            printf("Seed found for your windset %s.\n Use %s as your Dolphin custom RTC time, and make sure you choose the right difficulty.",
    //                targetWind.toString().c_str(), OSCalendarTimeToDolphinRTC(ctime).c_str());
    //        }

    //        return 0;
    //    }

    //    // Get winds at seed
    //    if (_stricmp(argv[i], "-s") == 0)
    //    {
    //        RPGolWindSet wind;
    //        
    //        // Get input seed
    //        std::string seedstr = argv[++i];
    //        u32 seed = std::stoul(seedstr, nullptr, 16);

    //        getWindSetFromSeed(seed, wind);
    //        std::printf("Seed: 0x%X -> %s", seed, wind.toString().c_str());

    //        return 0;
    //    }

    //    // Dump all Dolphin-usable wind seeds (No msec/usec)
    //    if (_stricmp(argv[i], "-dump-dolphin") == 0)
    //    {
    //        std::printf("Dumping all Dolphin-usable wind sets to wind_dolphin.csv...\n");
    //        dumpWindSeeds(false);
    //    }

    //    // Dump all possible wind sets (0x00000000 -> 0xFFFFFFFF)
    //    if (_stricmp(argv[i], "-dump-all") == 0)
    //    {
    //        std::printf("Are you sure you want to dump ALL possible seeds?\n");
    //        std::printf("This will take ~116GB of space.\n");
    //        std::printf("Would you like to continue with the dump? (Y/N)");

    //        char c;
    //        std::cin >> c;
    //        if (std::toupper(c) == 'N') break;

    //        std::printf("Dumping all possible wind sets to wind_all.csv...\n");
    //        dumpWindSeeds(true);
    //    }
    //}

    return 0;
}

/// <summary>
/// Finds Dolphin-usable seed with the closest wind set to the target.
/// </summary>
/// <param name="target">Target (ideal) wind set</param>
/// <param name="out">Calendar time output</param>
/// <returns>True if the target was found, otherwise false</returns>
bool tryToFindSeedFromWindSet(const RPGolWindSet& target, OSCalendarTime& out)
{
    //// Highest wind set score so far
    //u32 bestScore = 0;
    //// Best wind set found so far
    //RPGolWindSet bestWind;

    //for (u32 i = 0; i < 0x10000; i++)
    //{
    //    // Init seed
    //    RPUtlRandom::setSeed(i);
    //    // Generate new wind set
    //    RPGolWindSet newWind = RPGolConfig::getInstance()->getWindSet();

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
    //for (u32 i = 0; i < 60; i++) // min
    //{
    //    for (u32 j = 0; j < 60; j++) // sec
    //    {
    //        // Setup calendar time for seed
    //        out.min = i;
    //        out.sec = j;

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

    //std::printf("A seed for %s could not be found.\n The closest seed is %s, which gave the wind set\n%s.",
    //    target.toString().c_str(), OSCalendarTimeToDolphinRTC(out).c_str(), bestWind.toString().c_str());

    return false;
}

void getWindSetFromSeed(u32 seed, RPGolWindSet& out)
{
    //RPGolConfig* pInstance = RPGolConfig::getInstance();

    //RPUtlRandom::setSeed(seed);
    //pInstance->chooseWindSet();
    //out = pInstance->getWindSet();
}

void dumpWindSeeds(bool bNonRtc)
{
    //RPGolConfig* pInstance = RPGolConfig::getInstance();
    //RPGolWindSet wind;
    //OSCalendarTime time;

    //// Non-Dolphin RTC seeds allowed
    //if (bNonRtc)
    //{
    //    std::ofstream oStrm("wind_all.csv", std::ios::trunc);
    //    for (u32 i = 0x00000000; i < 0xFFFFFFFF; i++)
    //    {
    //        // Next line of output
    //        std::string line;

    //        // No need to calculate seed with initialize
    //        RPUtlRandom::setSeed(i);
    //        line += "\"" + u32ToHexStr(i) + "\", ";
    //        
    //        // Generate wind
    //        pInstance->chooseWindSet();

    //        // Save result
    //        wind = pInstance->getWindSet();
    //        // Comma delimiter for CSV
    //        line += wind.toString("\"", "\"");

    //        // Write to CSV
    //        line += "\n";
    //        oStrm.write(line.c_str(), line.length());
    //    }

    //    oStrm.close();
    //    return;
    //}

    //// Dolphin RTC seeds only
    //std::ofstream oStrm("wind_dolphin.csv", std::ios::trunc);
    //// CSV column titles
    //std::string csvHeader("Clock Min, Clock Sec, Seed, ");
    //csvHeader += "Hole 1, Hole 2, Hole 3, Hole 4, Hole 5, Hole 6, Hole 7, Hole 8, Hole 9\n";
    //oStrm.write(csvHeader.c_str(), csvHeader.length());
    //// Dolphin doesnt support msec/usec
    //for (u32 i = 0; i < 60; i++) // min
    //{
    //    for (u32 j = 0; j < 60; j++) // sec
    //    {
    //        // Next line of output
    //        std::string line;

    //        // Setup time
    //        time.min = i;
    //        time.sec = j;
    //        time.msec = 603;
    //        time.usec = 591;

    //        line += std::to_string(i) + ",";
    //        line += std::to_string(j) + ",";

    //        // Init random
    //        RPUtlRandom::initialize(time);
    //        line += u32ToHexStr(RPUtlRandom::getSeed()) + ",";

    //        // Generate wind
    //        pInstance->chooseWindSet();

    //        // Save result
    //        wind = pInstance->getWindSet();
    //        line += wind.toString("", "", "\"", "\",", true);

    //        // Write to CSV
    //        line += "\n";
    //        oStrm.write(line.c_str(), line.length());
    //    }
    //}

    //oStrm.close();
}