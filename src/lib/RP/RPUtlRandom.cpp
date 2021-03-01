#include <lib/rvl/OSTime.h>
#include "RPUtlRandom.h"
#include <cstdio>

#ifdef __MWERKS__
void RPUtlRandom::initialize()
{
	INSTANCE_GUARD(RPUtlRandom);
	// Get console time
	u64 rtc = OSGetTime();

	// Convert RTC time to calendar time
	OSCalendarTime ctime;
	OSTicksToCalendarTime(rtc, &ctime);

	// Manipulate ctime data to make up seed
	setSeed(ctime.min << 26 | ctime.sec << 20 | ctime.msec << 10 | ctime.usec);
}
#else
void RPUtlRandom::initialize(u64 rtc)
{
	// Convert RTC time to calendar time
	OSCalendarTime ctime;
	OSTicksToCalendarTime(rtc, &ctime);

	// Manipulate ctime data to make up seed
	setSeed(ctime.min << 26 | ctime.sec << 20 | ctime.msec << 10 | ctime.usec);
}

void RPUtlRandom::initialize(const OSCalendarTime& ctime)
{
	setSeed(ctime.min << 26 | ctime.sec << 20 | ctime.msec << 10 | ctime.usec);
}
#endif

void RPUtlRandom::setSeed(u32 seed)
{
	std::printf("[RPUtlRandom::setSeed] New seed %#.8x\n", seed);

	INSTANCE_GUARD(RPUtlRandom);
	mInstance->mSeed = seed;
}

u32 RPUtlRandom::calc()
{
	INSTANCE_GUARD(RPUtlRandom);

	// Evaluate 64 bits and save lower 32 to replicate
	// 750 overflow arithmetic
	u64 seed = mInstance->mSeed * SEED_STEP + 1;
	mInstance->mSeed = (u32)seed;

	std::printf("[RPUtlRandom::calc] New seed -> %#.8x\n", mInstance->mSeed);
	return mInstance->mSeed;
}

u32 RPUtlRandom::getU32()
{
	return calc() >> 16;
}

RPUtlRandom* RPUtlRandom::getInstance()
{
	INSTANCE_GUARD(RPUtlRandom);
	return mInstance;
}

RPUtlRandom *RPUtlRandom::mInstance = NULL;
