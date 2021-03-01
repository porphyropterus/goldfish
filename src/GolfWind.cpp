#include "GolfWind.h"
#include <cstdio>
#include "lib/RP/RPUtlRandom.h"

void makeWindSet(const DifficultyInfo& diff)
{
	// Simulate all random number generations before wind is generated
	for (int i = 0; i < CALC_BEFORE_WIND; i++)
	{
		RPUtlRandom::calc();
	}

	printf("Making wind dirs...\n");

	u32 *windDirs = new u32[WIND_DIR_ARRAY_SIZE];
	//makeRandArrayU32(WIND_DIR_ARRAY_SIZE, windDirs, RPGlfDefine::MAX_WIND_DIV);

	//std::printf("Wind dirs:\n");
	//for (int i = 0; i < WIND_DIR_ARRAY_SIZE; i++)
	//{
	//	std::printf("%#.4x, ", windDirs[i]);
	//}
}
