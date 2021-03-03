#include <iostream>
#include "types.h"
#include <lib/RP/RPUtlRandom.h>
#include <lib/RP/RPGlfConfig.h>
#include <lib/rvl/OSTime.h>

///////////////////////
//#define DEBUG      //
//#define __DO_TESTS //
//#define __CTIME    //
#define __WIND_GEN //
///////////////////////

int main(u32 argc, char** argv)
{
	OSCalendarTime ctime;
	ctime.hour = NULL;
	ctime.mday = NULL;
	ctime.mon = NULL;
	ctime.year = NULL;
	ctime.wday = NULL;
	ctime.yday = NULL;

	// Dolphin only supports RTC up to the precision of seconds.
	// As a result, the millisecond and microsecond fields
	// of the calendar time will always have the same arbitrary values.
	ctime.msec = DOLPHIN_MSEC;
	ctime.usec = DOLPHIN_USEC;

#ifdef __WIND_GEN
	u32 correctDirs[] = {
		0x0002, 0x0003, 0x0007, 0x0004, 0x0000, 0x0001, 0x0006, 0x0005, 0x0000
	};
	s32 correctSpeeds[] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8
	};

	for (u32 i = 0; i < 0xFFFFFFFF; i++)
	{
		//RPUtlRandom::setSeed(0x11494C6F);
		RPUtlRandom::setSeed(i);
		RPGlfConfig::chooseWindSet(diff_Ninehole);

		Wind* pWinds = RPGlfConfig::getWinds();

		bool speed_correct = true, dir_correct = true;

		// std::printf("Testing wind speeds...\n");
		for (int i = 0; i < RPGlfDefine::HOLE_SIZE - 1; i++)
		{
			if (pWinds[i].mSpeed != correctSpeeds[i])
			{
				speed_correct = false;
				break;
				// printf("[ERROR] Expected %#.4x at pWindSpeeds[%d], got %#.4x.\n", correctSpeeds[i], i, pWinds[i].mSpeed);
			}
		}
		if (speed_correct)
		{
			printf("OK (%#.8x)\n", i);
		}
	}

	//std::printf("\nTesting wind directions...\n");
	//for (int i = 0; i < RPGlfDefine::HOLE_SIZE; i++)
	//{
	//	if (pWinds[i].mDirection != correctDirs[i])
	//	{
	//		dir_correct = false;
	//		printf("[ERROR] Expected %s at pWindDirs[%d], got %s.\n", windDirStrings[correctDirs[i]].c_str(), i, windDirStrings[pWinds[i].mDirection].c_str());
	//	}
	//}
	//if (dir_correct)
	//{
	//	printf("OK\n");
	//}

#endif
#ifdef __CTIME
	// 04:20
	ctime.sec = 32;
	ctime.min = 02;

	ctime.hour = NULL;
	ctime.mday = NULL;
	ctime.mon = NULL;
	ctime.year = NULL;
	ctime.wday = NULL;
	ctime.yday = NULL;

	// Dolphin only supports RTC up to the precision of seconds.
	// As a result, the millisecond and microsecond fields
	// of the calendar time will always have the same arbitrary values.
	ctime.msec = DOLPHIN_MSEC;
	ctime.usec = DOLPHIN_USEC;

	RPUtlRandom::initialize(ctime);
	RPGlfConfig::chooseWindSet(diff_Ninehole);

	s32* pSpeeds = RPGlfConfig::getWindSpeeds();
	u32* pDirs = RPGlfConfig::getWindDirs();

	for (int i = 0; i < RPGlfDefine::HOLE_SIZE; i++)
	{
		std::printf("Hole %d: {%dmph, %s}\n", (i + 1), (pSpeeds[i] * 2), windDirStrings[pDirs[i]].c_str());
	}

	while (true);

#endif

#ifdef DEBUG
	u32 correctDirs[] = {
		0x0002, 0x0003, 0x0007, 0x0004, 0x0000, 0x0001, 0x0006, 0x0005, 0x0000
	};
	s32 correctSpeeds[] = {
		0x000F, 0x0004, 0x0003, 0x000A, 0x000C, 0x0008, 0x000B, 0x0006, 0x0000
	};

	RPUtlRandom::setSeed(0x11494C6F);
	//RPUtlRandom::setSeed(0x01f78a40);
	RPGlfConfig::chooseWindSet(diff_Ninehole);

	Wind* pWinds = RPGlfConfig::getWinds();

	bool speed_correct = true, dir_correct = true;

	std::printf("Testing wind speeds...\n");
	for (int i = 0; i < RPGlfDefine::HOLE_SIZE; i++)
	{
		if (pWinds[i].mSpeed != correctSpeeds[i])
		{
			speed_correct = false;
			printf("[ERROR] Expected %#.4x at pWindSpeeds[%d], got %#.4x.\n", correctSpeeds[i], i, pWinds[i].mSpeed);
		}
}
	if (speed_correct)
	{
		printf("OK\n");
	}

	std::printf("\nTesting wind directions...\n");
	for (int i = 0; i < RPGlfDefine::HOLE_SIZE; i++)
	{
		if (pWinds[i].mDirection != correctDirs[i])
		{
			dir_correct = false;
			printf("[ERROR] Expected %s at pWindDirs[%d], got %s.\n", windDirStrings[correctDirs[i]].c_str(), i, windDirStrings[pWinds[i].mDirection].c_str());
		}
	}
	if (dir_correct)
	{
		printf("OK\n");
	}

#else
#ifdef __CTIME
	RPUtlRandom::setSeed(ctime.min << 26 | ctime.sec << 20 | ctime.msec << 10 | ctime.usec);
#endif
#endif

	// RPGlfConfig::makeWindSet(diff_Ninehole, NULL, NULL);

#ifdef __DO_TESTS
	// Seed for testing
	RPUtlRandom::setSeed(0x04094C6F);

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

	std::printf("\n\nTesting array 2...\n");
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
