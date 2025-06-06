#pragma once

#include "types.h"

class Sp2Rand
{
public:
    void initialize(u32 seed)
    {
        seed_top = seed;
    }

    void advance(u32 n = 1)
    {
        for (u32 i = 0; i < n; i++)
        {
            Rand();
        }
    }

    u32 Rand()
    {
        seed_top = seed_top * 0x41C64E6D + 0x3039;

        /*
        seed_bot does not matter for our purposes
        */

        // if (seed_bot & 1)
        // {
        //     seed_bot = seed_bot ^ 0x11020;
        // }
        // seed_bot = seed_bot >> 1;

        /*
        neither does xoring it with seed_top,
        the bottom 16 bits of the seed are not used
        */

        return seed_top /*^ seed_bot*/;
    }

    f32 getF32()
    {
        // Limited to u16 bounds
        const u16 rnd = static_cast<u16>(0xFFFF & (Rand() >> 16));
        // Convert to float
        const f32 rnd_f = static_cast<f32>(rnd);
        // Convert to percentage
        return rnd_f / static_cast<f32>(0xFFFF + 1);
    }

private:
    u32 seed_top;
    u32 seed_bot;
};