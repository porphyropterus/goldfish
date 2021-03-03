#include "RPGlfConfig.h"
#include <lib/PPC.H>
#include <lib/RP/RPUtlRandom.h>
#include <cstdio>

/// <summary>
/// 
/// </summary>
/// <param name="max"></param>
/// <param name="pArray"></param>
void RPGlfConfig::makeRandomSequence(s32 max, s32* pArray)
{
	u32 r0;

	// Initialize array
	for (u32 i = 0; i < max; i++)
	{
		pArray[i] = -max;
	}

	u32 it = 0;
	while (true)
	{
		s32 signedResult = (s32)(RPUtlRandom::getF32() * (max - it));

		s32 searchStart = 0;
		u32 r5 = 0;

		if (max > 0)
		{
			for (s32 i = 0; i < max; i++)
			{
				if ((pArray[searchStart] < 0) && (--signedResult < 0))
				{
					rlwinm(signedResult, r5, 2, 0x3FFFFFFF);

					pArray[signedResult / sizeof(s32)] += max;
					pArray[it] += r5;

					if (++it >= max)
					{
						return;
					}
					
					break;
				}
				else
				{
					searchStart++;
					r5++;
				}
			}
		}
	}
}

/// <summary>
/// Generate a pool of random speeds and directions, and choose 9 of each
/// to represent the "wind set" for the current game.
/// </summary>
/// <param name="diff"> Selected difficulty </param>
void RPGlfConfig::chooseWindSet(const DifficultyInfo& diff)
{
	// Simulate all random number generations before wind is generated
	RPUtlRandom::advance(CALC_BEFORE_WIND);

	// Generate random arrays (values this function will hand-pick from)
	s32 randomDirs[randomDirArraySize];
	//makeRandomArray(randomDirArraySize, randomDirs);
	s32 randomSpeeds[randomSpeedArraySize];
	//makeRandomArray(randomSpeedArraySize, randomSpeeds);

	u32 gameLength = diff.endHole - diff.startHole;
}

RPGlfConfig* RPGlfConfig::getInstance()
{
	INSTANCE_GUARD(RPGlfConfig);
	return mInstance;
}

RPGlfConfig* RPGlfConfig::mInstance = NULL;
