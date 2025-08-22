#pragma once

#include "AbstractFinder.h"
#include "BlinkGroup.h"

#include "lib/RP/RPKokeshiBlinkMgr.h"

#include <cmath>

class OgBlinkFinderOutput
{
public:
    u32 seed;
};

class OgBlinkFinder : public AbstractFinder<BlinkGroup, OgBlinkFinderOutput>
{
public:
    OgBlinkFinder(const std::string &filePath)
        : AbstractFinder<BlinkGroup, OgBlinkFinderOutput>(static_cast<u32>(std::pow(RPKokeshiBlinkMgr::NUM_UNIQUE_BLINK_TIMES, BlinkGroup::NUM_BLINKS_PER_HASH)), filePath) {}

    virtual ~OgBlinkFinder() = default;

    virtual std::vector<u32> inputToHashes(const BlinkGroup &input) override
    {
        return {input.toHash()};
    }

    virtual OgBlinkFinderOutput generatePotentialOutputFromSeed(u32 seed) override
    {
        return OgBlinkFinderOutput{seed};
    }

    virtual u32 nextSeed(u32 currentSeed) override
    {
        return currentSeed + 1;
    }
};