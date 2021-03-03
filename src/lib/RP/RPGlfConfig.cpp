#include "RPGlfConfig.h"
#include <lib/PPC.H>
#include <lib/RP/RPUtlRandom.h>
#include <cstdio>

void RPGlfConfig::makeRandomWind()
{
	// Simulate all random number generations before wind is generated
	RPUtlRandom::advance(CALC_BEFORE_WIND);

	
}

// 8029dea8
// I'd like to apologize in advance for the headache you are about to suffer
// from reading this code
void RPGlfConfig::makeRandomArray(s32 val, s32* pArray)
{
	s32* pArrayStart = &pArray[0];
	s32* pArrayCopy = pArray;
	s32* pArrayCopy2 = pArrayStart;

	u32 r0;

	if ((val > 0) && (val < S32_MAX))
	{
		if (val > 8)
		{
			for (u32 i = 0; i < (val - 1) / 8; i++)
			{
				pArray[0 + i] = -val;
				pArray[1 + i] = -val;
				pArray[2 + i] = -val;
				pArray[3 + i] = -val;
				pArray[4 + i] = -val;
				pArray[5 + i] = -val;
				pArray[6 + i] = -val;
				pArray[7 + i] = -val;
			}
		}

		r0 = val - (val - 1) / 8;
		for (u32 i = 0; i < r0; i++)
		{
			pArray[i] = -val;
		}
	}

	u32 r31 = 0;
	u32 r25 = 0;
	u32 r26 = 0;
	while (true)
	{
	generateAgain:
		u32 result = (u32)(RPUtlRandom::getF32() * (val - r26));
		s32 signedResult = (s32)result;

		pArrayStart = &pArray[0];
		u32 r5 = 0;

		if (val > 0)
		{
			for (s32 i = 0; i < val; i++)
			{
				if ((*pArrayStart < 0) && (--signedResult < 0))
				{
					rlwinm(signedResult, r5, 2, 0x3FFFFFFF);
					r26++;

					r0 = pArrayCopy[signedResult / sizeof(s32)];
					r0 += val;
					pArrayCopy[signedResult / sizeof(s32)] = r0;

					r0 = pArrayCopy[r31];
					r0 += r5;
					pArrayCopy[r31] = r0;

					r25++;
					r31++;

					if (r25 >= val)
					{
						return;
					}
					else
					{
						goto generateAgain;
					}
				}
				else
				{
					pArrayStart++;
					r5++;
				}
			}
		}
	}
}

// 8029dcf4
// Array ptrs should be data filled by makeRandWind
// The final winds go in RPGlfConfig's member arrays
void RPGlfConfig::makeWindSet(const DifficultyInfo& diff, const u32 *pRandDirs, const s32 *pRandSpeeds)
{
	u32 gameLength = diff.endHole - diff.startHole;

}