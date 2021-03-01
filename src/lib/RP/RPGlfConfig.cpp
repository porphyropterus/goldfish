#include "RPGlfConfig.h"
#include <lib/RP/RPUtlRandom.h>
#include <cstdio>

void RPGlfConfig::makeRandomWind()
{
	// Simulate all random number generations before wind is generated
	for (int i = 0; i < CALC_BEFORE_WIND; i++)
	{
		RPUtlRandom::calc();
	}

	printf("Making wind dirs...\n");
	for (int i = 0; i < RPGlfDefine::MAX_WIND_DIV; i++)
	{
		
	}
}

void RPGlfConfig::makeWindSet(const DifficultyInfo& diff, const u32 *pDirections, const u32 *pSpeeds)
{
	u32 gameLength = diff.endHole - diff.startHole;

	// assign loop var
	u32 loopCount = 0;

	u32 nextSpeed;
	// Loop through each hole (0-8)
	for (u32 i = 0; i < RPGlfDefine::HOLE_SIZE; i++)
	{
		// If hole 'i' will not be played in the selected difficulty,
		// it will have dummy wind (32 S)
		if ((i < diff.startHole) || (i > diff.endHole))
		{
			windDirections[i] = RPGlfDefine::MAX_WIND_DIV; // Treated as South anyways for some reason
			windSpeeds[i] = RPGlfDefine::MAX_WIND_SPD; // m/s
		}
		// Otherwise, attempt to find a wind direction + speed
		// that is within the difficulty's limits (DifficultyInfo)
		else
		{
			// loop through "some list", until a wind speed is found that is within the gamemode limits
			// see function args 3 and 4
			do
			{
				do
				{
					nextSpeed = pSpeeds[loopCount];
					loopCount++;
				} while (nextSpeed < diff.minWind);
			} while (nextSpeed > diff.maxWind);

			// we found a wind speed within the gamemode limits!!
			windSpeeds[i] = nextSpeed;


			// once 8 holes have a wind speed above zero, the next one must be zero
			if (holesWithNonZeroSPD < 8)
			{
				windDirs[i] = local_d8[dirIndex];

				if (nextSpeed > 0)
				{
					// direction is irrelevant when speed is zero, so only increase the index when speed is above zero
					dirIndex++;

					holesWithNonZeroSPD++;
				}
			}
			else
			{
				if (nextSpeed == 0) windDirs[i] = SOUTH;
				else
				{
					// Random call to decide which hole gets 0 wind
					g_GolfRNG1 = g_GolfRNG1 * 0xffff + 1;
					u32 rngStep = g_GolfRNG1 >> 0x10;
					iVar4 = ((u32)(gameLength * rngStep / 0x10000));
					uVar5 = windDirs[startHole + iVar4];
					windSpeeds[startHole + iVar4] = 0;
					windDirs[i] = (WindDirection)uVar5;
				}
			}
		}
	}
}