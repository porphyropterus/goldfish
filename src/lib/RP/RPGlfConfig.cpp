#include "RPGlfConfig.h"
#include <lib/PPC.H>
#include <lib/RP/RPUtlRandom.h>
#include <cstdio>

void RPGlfConfig::makeRandomWind()
{
	// Simulate all random number generations before wind is generated
	for (int i = 0; i < CALC_BEFORE_WIND; i++)
	{
		RPUtlRandom::calc();
	}

	
}

// 8029dea8
void RPGlfConfig::makeRandArray(s32 val, u32* pArray)
{
	u32* pArrayCopy = pArray;

	u32 r0;
	s32 r8 = 0;

	if (val > 0)
	{
		s32 r6 = val - 8;

		//  ^ Yep...
		if (val > 8)
		{
			// Yeah, I don't get this either
			if ((val >= 0) && (val < S32_MAX))
			{
				s32 r0 = r6 + 7;
				rlwinm(r0, r0, 0x1D, 0xFFFFFFF8);

				s32 r7 = -val;

				if (r6 > 0)
				{
					for (int i = 0; i < r0; i++)
					{
						pArray[0 * i] = r7;
						pArray[1 * i] = r7;
						pArray[2 * i] = r7;
						pArray[3 * i] = r7;
						pArray[4 * i] = r7;
						pArray[5 * i] = r7;
						pArray[6 * i] = r7;
						pArray[7 * i] = r7;
						pArray[8 * i] = r7;
						r8 += 8;
					}
				}
			}
		}

		s32 r5;
		rlwinm(r5, r8, 2, 0x3FFFFFFF);

		r0 = val - r8;
		pArray += r5;
		r5 = -val;

		if (r8 < val)
		{
			for (s32 i = 0; i < r0; i++)
			{
				*pArray = r5;
				pArray++;
			}
		}
	}


	while (true)
	{
		u32* r31 = pArrayCopy;
		u32 r25 = 0;
		u32 r26 = 0;


		if (r25 >= val)
		{
			return;
		}

		s32 result = (s32)(RPUtlRandom::getF32() * -val);

		s32 r5 = 0;

		if (val > 0)
		{
			for (s32 i = 0; i < val; i++)
			{
				if (pArray[0] < 0)
				{
					if (--val < 0)
					{
						rlwinm(val, r5, 2, 0x3FFFFFFF);
						r26++;

						r0 = pArrayCopy[val / sizeof(s32)];
						r0 += val;
						pArrayCopy[val / sizeof(s32)] = r0;

						r0 = pArrayCopy[i];
						r0 += r5;
						pArrayCopy[i] = r0;

						r25++;

						if (r25 >= val)
						{
							return;
						}
					}
				}
			}
		}
	}
}

// 8029dcf4, but MakeRandArray calls are split into makeRandomWind + makeRandArray
// Array ptrs should be data from makeRandomWind
// The final winds go in RPGlfConfig's member arrays
void RPGlfConfig::makeWindSet(const DifficultyInfo& diff, const u32 *pRandDirs, const u32 *pRandSpeeds)
{
	u32 gameLength = diff.endHole - diff.startHole;

}