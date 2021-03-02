#include <lib/rvl/OSTime.h>
#include "RPUtlRandom.h"
#include <cstdio>

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

void RPUtlRandom::setSeed(u32 seed)
{
	std::printf("[RPUtlRandom::setSeed] New seed %#.8x\n", seed);
	getInstance()->mSeed = seed;
}

u32 RPUtlRandom::calc()
{
	RPUtlRandom* instance = getInstance();

	// Evaluate 64 bits and save lower 32 to replicate
	// multiply low overflow arithmetic
	u64 seed = instance->mSeed * SEED_STEP + 1;
	instance->mSeed = (u32)seed;

	std::printf("[RPUtlRandom::calc] New seed -> %#.8x\n", instance->mSeed);
	return instance->mSeed;
}

u32 RPUtlRandom::getU32()
{
	return calc() >> 16;
}

f32 RPUtlRandom::getF32()
{
	return (float)getU32() / 0x10000;
}

RPUtlRandom* RPUtlRandom::getInstance()
{
	INSTANCE_GUARD(RPUtlRandom);
	return mInstance;
}

RPUtlRandom *RPUtlRandom::mInstance = NULL;
