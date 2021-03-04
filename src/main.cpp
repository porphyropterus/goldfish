#include <iostream>
#include "types.h"
#include <lib/RP/RPUtlRandom.h>
#include <lib/RP/RPGlfConfig.h>
#include <lib/RP/RPGlfWindSet.h>
#include "WindArgParser.h"
#include <lib/rvl/OSTime.h>

static DifficultyInfo g_Diff;

int main(u32 argc, char** argv)
{
	RPGlfWindSet targetWind;
	OSCalendarTime ctime;

	// 9 hole default difficulty
	g_Diff = diff_Ninehole;

	for (u32 i = 0; i < argc; i++)
	{
		// Parse difficulty
		if (stricmp(argv[i], "-d") == 0)
		{
			switch (*argv[i + 1])
			{
			case 'B':
				g_Diff = diff_Beginner;
				break;
			case 'I':
				g_Diff = diff_Intermediate;
				break;
			case 'E':
				g_Diff = diff_Expert;
				break;
			case 'N':
				g_Diff = diff_Ninehole;
				break;
			}
		}

		if (stricmp(argv[i], "-w") == 0)
		{
			WindArgParser::parseTargetWindSet(std::string(argv[++i]), targetWind);
			if (tryToFindSeedFromWindSet(targetWind, ctime))
			{
				printf("Seed found for your windset %s.\n Use %s as your Dolphin custom RTC time, and make sure you choose the right difficulty.",
					targetWind.toString().c_str(), OSCalendarTimeToDolphinRTC(ctime).c_str());
			}
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
	for (u32 i = 0; i < 60; i++)
	{
		for (u32 j = 0; j < 60; j++)
		{
			// Setup calendar time for seed
			out.min = i;
			out.sec = j;
			// Dolphin RTC has a precision of seconds, while the Wii supports milli/microseconds
			// As a result, any Dolphin RTC time will always have a specific arbitrary value for milli/microseconds
			// Unfortunately, this limits us from (60*60*1000*1000) seeds to only (60*60) when TASing.
			out.msec = DOLPHIN_MSEC;
			out.usec = DOLPHIN_USEC;

			// Init seed
			RPUtlRandom::initialize(out);
			// Generate new wind set
			RPGlfConfig::chooseWindSet(g_Diff);
			RPGlfWindSet newWind = RPGlfConfig::getWindSet();
			// If the newest wind set is closer to the target than the 
			// best wind set, the new wind set becomes the best wind set
			u32 score = newWind.scoreAgainstTarget(target);
			if (score > bestScore)
			{
				bestScore = score;
				std::memcpy(&bestWind, &newWind, sizeof(bestWind));
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
		target.toString().c_str(), OSCalendarTimeToDolphinRTC(out), bestWind.toString());
}
