#pragma once

#include "types.h"

struct OSCalendarTime;

class RPUtlRandom
{
public:
    static RPUtlRandom *GetInstance()
    {
        static RPUtlRandom instance;
        return &instance;
    }

    static void initialize(const OSCalendarTime &); // at 801bea28
    static void initialize(u32);

    static u32 getSeed();
    static void advance(u32);
    static void unadvance(u32);

    static u32 getU32();
    static f32 getF32(bool ver_1_0 = false);

private:
    RPUtlRandom() : mSeed(0) {}
    ~RPUtlRandom() {}

    static u32 calc();

private:
    u32 mSeed; // at 804bf688

    static constexpr u32 SEED_STEP = 69069;
    static constexpr u32 SEED_STEP_INV = 0xA5E2A705; // modular inverse of 69069 mod 2^32
};
