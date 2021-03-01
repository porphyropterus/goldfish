#include "GolfWind.h"
#include "lib/RP/RPUtlRandom.h"

void makeWindSet(const DifficultyInfo& diff)
{
	// Simulate all random number generations before wind is generated
	for (int i = 0; i < CALC_BEFORE_WIND; i++)
	{
		RPUtlRandom::calc();
	}


}

void makeRandArrayU32(u32 length, u32 *pArray)
{

}