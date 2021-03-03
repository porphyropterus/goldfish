#include <iostream>
#include "types.h"
#include <lib/RP/RPUtlRandom.h>
#include <lib/RP/RPGlfConfig.h>
#include <lib/rvl/OSTime.h>

/////////////////////
#define DEBUG      //
#define __DO_TESTS //
/////////////////////

int main()
{
	OSCalendarTime ctime;

#ifdef DEBUG
	RPUtlRandom::setSeed(0x04094C6F);
#else
	RPUtlRandom::setSeed(ctime.min << 26 | ctime.sec << 20 | ctime.msec << 10 | ctime.usec);
#endif

	// RPGlfConfig::makeWindSet(diff_Ninehole, NULL, NULL);

#ifdef __DO_TESTS
	// Simulate all random number generations before wind is generated
	RPUtlRandom::advance(CALC_BEFORE_WIND);

	// Generate random array of wind directions
	s32 arrayTest1[8];
	RPGlfConfig::makeRandomSequence(randomDirArraySize, arrayTest1);
	// Generate random array of wind speeds
	s32 arrayTest2[16];
	RPGlfConfig::makeRandomSequence(randomSpeedArraySize, arrayTest2);

	bool correct = true, correct2 = true;
	s32 correctArray1[8] = { 0x0001, 0x0002, 0x0003, 0x0006, 0x0005, 0x0007, 0x0004, 0x0000 };
	s32 correctArray2[16] = { 0x0008, 0x000F, 0x000E, 0x0003, 0x0004, 0x0002, 0x0005, 0x000A,
			 0x0001, 0x0006, 0x0000, 0x0009, 0x000B, 0x0007, 0x000D, 0x000C };

	std::printf("Testing array 1...\n");
	for (int i = 0; i < 8; i++)
	{
		if (arrayTest1[i] != correctArray1[i])
		{
			correct = false;
			printf("[ERROR] Expected %#.4x at testArray1[%d], got %#.4x.\n", correctArray1[i], i, arrayTest1[i]);
		}
	}
	if (correct)
	{
		printf("OK\n");
	}

	std::printf("\nTesting array 2...\n");
	for (int i = 0; i < 16; i++)
	{
		if (arrayTest2[i] != correctArray2[i])
		{
			correct2 = false;
			printf("[ERROR] Expected %#.4x at testArray2[%d], got %#.4x.\n", correctArray2[i], i, arrayTest2[i]);
		}
	}
	if (correct2)
	{
		printf("OK");
	}

#endif
	return 0;
}
