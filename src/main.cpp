#include <iostream>
#include "types.h"
#include <lib/RP/RPUtlRandom.h>
#include <lib/RP/RPGlfConfig.h>
#include <lib/RP/RPGlfWindSet.h>
#include "DifficultyInfo.h"
#include "WindArgParser.h"
#include <lib/rvl/OSTime.h>

bool tryToFindSeedFromWindSet(const RPGlfWindSet& target, OSCalendarTime& out);
void getWindSetFromSeed(u32 seed, RPGlfWindSet& out);

int main(u32 argc, char** argv)
{
    RPGlfWindSet targetWind;
    OSCalendarTime ctime;

    // 9 hole default difficulty
    DifficultyInfo d = diff_Ninehole;

    for (u32 i = 0; i < argc; i++)
    {
        // Parse difficulty
        if (_stricmp(argv[i], "-d") == 0)
        {
            switch (*argv[++i])
            {
            case 'B':
                d = diff_Beginner;
                break;
            case 'I':
                d = diff_Intermediate;
                break;
            case 'E':
                d = diff_Expert;
                break;
            case 'N':
                d = diff_Ninehole;
                break;
            }
        }

        // Set chosen difficulty
        RPGlfConfig::getInstance()->setDifficulty(d);

        if (_stricmp(argv[i], "-w") == 0)
        {
            WindArgParser::parseTargetWindSet(std::string(argv[++i]), targetWind);
            if (tryToFindSeedFromWindSet(targetWind, ctime))
            {
                printf("Seed found for your windset %s.\n Use %s as your Dolphin custom RTC time, and make sure you choose the right difficulty.",
                    targetWind.toString().c_str(), OSCalendarTimeToDolphinRTC(ctime).c_str());
            }
        }

        // Get winds at seed
        if (_stricmp(argv[i], "-s") == 0)
        {
            RPGlfWindSet wind;
            
            // Get input seed
            std::string seedstr = argv[++i];
            u32 seed = std::stoi(seedstr, nullptr, 16);

            getWindSetFromSeed(seed, wind);
            std::printf("Seed: 0x%X -> %s", seed, wind.toString().c_str());
        }
    }

    return 0;
}

/// <summary>
/// Finds Dolphin-usable seed with the closest wind set to the target.
/// </summary>
/// <param name="target">Target (ideal) wind set</param>
/// <param name="out">Calendar time output</param>
/// <returns>True if the target was found, otherwise false</returns>
bool tryToFindSeedFromWindSet(const RPGlfWindSet& target, OSCalendarTime& out)
{
    // Highest wind set score so far
    u32 bestScore = 0;
    // Best wind set found so far
    RPGlfWindSet bestWind;

    // Nested loop to try all possible Dolphin RTC seeds
    for (u32 i = 0; i < 60; i++) // min
    {
        for (u32 j = 0; j < 60; j++) // sec
        {
            // Setup calendar time for seed
            out.min = i;
            out.sec = j;

            // Dolphin RTC has a precision of seconds due to it's usage of time_t.
            // As a result, when recording or playing back a movie, the millisecond and microsecond values will be set to zero.
            // If no movie is being recorded/played, the values are initialized to that of your computer's system time.
            // Because this tool is designed for TASing, the values will be initialized to zero here.
            out.msec = 0;
            out.usec = 0;

            // Init seed
            RPUtlRandom::initialize(out);
            // Generate new wind set
            RPGlfWindSet newWind = RPGlfConfig::getInstance()->getWindSet();

            // If the newest wind set is closer to the target than the 
            // best wind set, the new wind set becomes the best wind set
            u32 score = newWind.scoreAgainstTarget(target);
            if (score > bestScore)
            {
                bestScore = score;
                bestWind = newWind;
            }

            // If it is a perfect match, we don't need to search anymore
            if (score == perfectScore)
            {
                std::printf("Match found !\n");
                return true;
            }
        }
    }

    std::printf("A seed for %s could not be found.\n The closest seed is %s, which gave the wind set\n%s.",
        target.toString().c_str(), OSCalendarTimeToDolphinRTC(out).c_str(), bestWind.toString().c_str());

    return false;
}

void getWindSetFromSeed(u32 seed, RPGlfWindSet& out)
{
    RPGlfConfig* pInstance = RPGlfConfig::getInstance();

    RPUtlRandom::setSeed(seed);
    pInstance->chooseWindSet();
    out = pInstance->getWindSet();
}