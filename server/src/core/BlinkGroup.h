#pragma once

#include <vector>

#include "lib/RP/RPKokeshiBlinkMgr.h"

#include "types.h"

class BlinkGroup
{

public:
    BlinkGroup() = default;
    ~BlinkGroup() = default;

    BlinkGroup(const std::vector<u32> &blinkTimes) : blinkTimes(blinkTimes) {}

    BlinkGroup(u32 seed, u32 depth)
    {
        RPUtlRandom::initialize(seed);

        RPUtlRandom::unadvance(depth + 1); // +1 = account for last blink that we don't see the wait time of

        for (u32 i = 0; i < depth; i++)
        {
            u32 time = RPKokeshiBlinkMgr::getRandomBlinkTime();
            blinkTimes.push_back(time);
        }
    }

    static constexpr u32 NUM_BLINKS_PER_HASH = 7;

    u32 toHash() const
    {
        if (blinkTimes.size() < NUM_BLINKS_PER_HASH)
        {
            throw std::invalid_argument("not enough blinks");
        }

        u32 hash = 0;

        // hash the last NUM_BLINKS_PER_HASH numbers
        for (u32 i = 0; i < NUM_BLINKS_PER_HASH; i++)
        {
            hash *= RPKokeshiBlinkMgr::NUM_UNIQUE_BLINK_TIMES;
            hash += RPKokeshiBlinkMgr::blinkTimeToUniqueIndex(this->blinkTimes[blinkTimes.size() - i - 1]);
        }

        return hash;
    }

    bool operator==(const BlinkGroup &other) const
    {
        return this->blinkTimes == other.blinkTimes;
    }

    size_t size() const
    {
        return blinkTimes.size();
    }

    static u32 compare(const BlinkGroup &a, const BlinkGroup &b)
    {
        u32 sum = 0;
        for (size_t i = 0; i < std::min(a.size(), b.size()); i++)
        {
            if (a.blinkTimes[i] != b.blinkTimes[i])
            {
                sum += std::abs(static_cast<s32>(a.blinkTimes[i]) - static_cast<s32>(b.blinkTimes[i]));
            }
        }
        return sum;
    }

private:
    std::vector<u32> blinkTimes;
};
