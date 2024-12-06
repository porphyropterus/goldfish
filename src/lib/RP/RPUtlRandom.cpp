#include "RPUtlRandom.h"
#include "lib/rvl/OSTime.h"

/// <summary>
/// Initialize seed using OSCalendarTime object
/// </summary>
/// <param name="ctime">OSCalendarTime reference</param>
void RPUtlRandom::initialize(const OSCalendarTime &ctime)
{
    GetInstance()->mSeed = (ctime.min << 26 | ctime.sec << 20 | ctime.msec << 10 | ctime.usec);
}

/// <summary>
/// Initialize seed using 32-bit seed
/// </summary>
/// <param name="seed">Seed value</param>
void RPUtlRandom::initialize(u32 seed)
{
    GetInstance()->mSeed = seed;
}

/// <summary>
/// Get RNG seed
/// </summary>
/// <returns>32-bit seed</returns>
u32 RPUtlRandom::getSeed()
{
    return GetInstance()->mSeed;
}

/// <summary>
/// Advance seed a specified number of steps
/// </summary>
/// <param name="n">Number of steps to advance</param>
void RPUtlRandom::advance(u32 n)
{
    for (u32 i = 0; i < n; i++)
        calc();
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
/// Advance seed one step forward, and return its value
/// </summary>
/// <returns>Seed after stepping forward one iteration</returns>
u32 RPUtlRandom::calc()
{
    RPUtlRandom *rng = GetInstance();

    // Linear congruential generator implementation
    // Old versions of glibc also use 69069 as the multiplier
    // u64 used to replicate overflow
    u64 seed = rng->mSeed * SEED_STEP + 1;
    return rng->mSeed = (u32)seed;
}