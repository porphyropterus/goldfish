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
	ctime.sec = 0;
	ctime.min = 1;
	ctime.hour = 6;
	ctime.mday = 3;
	ctime.mon = 4;
	ctime.year = 2020;
	ctime.wday = 0;
	ctime.yday = 123;
	ctime.msec = 595;
	ctime.usec = 111;

#ifdef DEBUG
	RPUtlRandom::setSeed(0x784B16BC);
#else
	RPUtlRandom::setSeed(ctime.min << 26 | ctime.sec << 20 | ctime.msec << 10 | ctime.usec);
#endif

	// RPGlfConfig::makeWindSet(diff_Ninehole, NULL, NULL);

	s32 arrayTest1[8];
	RPGlfConfig::makeRandomArray(8, arrayTest1);

	s32 arrayTest2[16];
	RPGlfConfig::makeRandomArray(16, arrayTest2);

	bool correct = true, correct2 = true;
	s32 correctArray1[8] = { 0x0001, 0x0002, 0x0003, 0x0006, 0x0005, 0x0007, 0x0004, 0x0000 };
	s32 correctArray2[16] = { 0x0008, 0x000F, 0x000E, 0x0003, 0x0004, 0x0002, 0x0005, 0x000A,
			 0x0001, 0x0006, 0x0000, 0x0009, 0x000B, 0x0007, 0x000D, 0x000C };

#ifdef __DO_TESTS
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
