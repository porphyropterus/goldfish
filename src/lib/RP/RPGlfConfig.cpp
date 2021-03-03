#include "RPGlfConfig.h"
#include <lib/PPC.H>
#include <lib/RP/RPUtlRandom.h>
#include <cstdio>

void RPGlfConfig::makeRandomArray(s32 len, s32* pArray)
{
	u32 r0;

	// Initialize array
	for (u32 i = 0; i < len; i++)
	{
		pArray[i] = -len;
	}

	u32 it = 0;
	while (true)
	{
		s32 signedResult = (s32)(RPUtlRandom::getF32() * (len - it));

		s32 searchStart = 0;
		u32 r5 = 0;

		if (len > 0)
		{
			for (s32 i = 0; i < len; i++)
			{
				if ((pArray[searchStart] < 0) && (--signedResult < 0))
				{
					rlwinm(signedResult, r5, 2, 0x3FFFFFFF);

					r0 = pArray[signedResult / sizeof(s32)];
					r0 += len;
					pArray[signedResult / sizeof(s32)] = r0;

					r0 = pArray[it];
					r0 += r5;
					pArray[it] = r0;

					if (++it >= len)
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

void RPGlfConfig::makeWindSet(const DifficultyInfo& diff)
{
	// Simulate all random number generations before wind is generated
	RPUtlRandom::advance(CALC_BEFORE_WIND);

	u32 gameLength = diff.endHole - diff.startHole;
}