#pragma once

#include "lib/RP/RPUtlRandom.h"

#include <stdexcept>

class RPKokeshiBlinkMgr
{
public:
    static constexpr u32 NUM_BLINK_TIMES = 10;
    static constexpr u32 NUM_UNIQUE_BLINK_TIMES = 9;
    static constexpr u32 sBlinkTimes[NUM_BLINK_TIMES] = {10, 10, 120, 180, 250, 260, 270, 280, 290, 300};

    static u32 getRandomBlinkIndex()
    {
        return RPUtlRandom::getU32() % NUM_BLINK_TIMES;
    }

    static u32 getRandomBlinkTime()
    {
        return sBlinkTimes[getRandomBlinkIndex()];
    }

    static u32 convertAbsoluteIndexToUniqueIndex(u32 idx)
    {
        if (idx >= NUM_BLINK_TIMES)
        {
            throw std::out_of_range("index out of range");
        }

        if (idx < (NUM_BLINK_TIMES - NUM_UNIQUE_BLINK_TIMES))
        {
            return 0;
        }
        else
        {
            return idx - (NUM_BLINK_TIMES - NUM_UNIQUE_BLINK_TIMES);
        }
    }

    static u32 blinkTimeToUniqueIndex(u32 idx)
    {
        for (u32 i = 0; i < NUM_BLINK_TIMES; i++)
        {
            if (sBlinkTimes[i] == idx)
            {
                return convertAbsoluteIndexToUniqueIndex(i);
            }
        }
        throw std::invalid_argument("invalid blink time");
    }
};