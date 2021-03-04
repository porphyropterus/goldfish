#include <lib/rvl/OSTime.h>
#include "RPUtlRandom.h"
#include <cstdio>
#include <cassert>

/// <summary>
/// Initialize seed using OSCalendarTime object
/// </summary>
/// <param name="ctime">OSCalendarTime reference</param>
void RPUtlRandom::initialize(const OSCalendarTime& ctime)
{
	setSeed(ctime.min << 26 | ctime.sec << 20 | ctime.msec << 10 | ctime.usec);
}

/// <summary>
/// Set seed to specified u32 value
/// </summary>
/// <param name="seed">Specified seed</param>
void RPUtlRandom::setSeed(u32 seed)
{
	//std::printf("[RPUtlRandom::setSeed] Seed init -> %#.8x\n", seed);
	getInstance()->mSeed = seed;
}

u32 RPUtlRandom::getSeed()
{
	return getInstance()->mSeed;
}

/// <summary>
/// Advance seed one step forward, and return its value
/// </summary>
/// <returns>Seed after stepping forward one iteration</returns>
u32 RPUtlRandom::calc()
{
	RPUtlRandom* instance = getInstance();

	// Evaluate 64 bits and save lower 32 to replicate
	// multiply low overflow arithmetic
	u64 seed = instance->mSeed * SEED_STEP + 1;
	return instance->mSeed = (u32)seed;
}

/// <summary>
/// Advance seed a specified number of steps
/// </summary>
/// <param name="n">Number of steps to advance</param>
void RPUtlRandom::advance(u32 n)
{
	for (u32 i = 0; i < n; i++)
	{
		calc();
	}
}

/// <summary>
/// Get random u32 value
/// </summary>
/// <returns>Random u32</returns>
u32 RPUtlRandom::getU32()
{
	return calc() >> 16;
}

/// <summary>
/// Get random float value
/// </summary>
/// <returns>Random f32</returns>
f32 RPUtlRandom::getF32()
{
	return (float)getU32() / 0x10000;
}

/// <summary>
/// Get static instance of RPUtlRandom class.
/// </summary>
/// <returns>RPUtlRandom instance</returns>
RPUtlRandom* RPUtlRandom::getInstance()
{
	INSTANCE_GUARD(RPUtlRandom);
	return mInstance;
}

RPUtlRandom *RPUtlRandom::mInstance = NULL;
