#pragma once

#include "TopNFinder.h"
#include "BlinkGroup.h"

#include "lib/RP/RPKokeshiBlinkMgr.h"

#include <cmath>

class OgBlinkFinderOutput
{
public:
    u32 seed;
    BlinkGroup blinkGroup;
};

class OgBlinkFinder : public TopNFinder<BlinkGroup, OgBlinkFinderOutput>
{
public:
    OgBlinkFinder(const std::string &filePath)
        : TopNFinder<BlinkGroup, OgBlinkFinderOutput>(static_cast<u32>(std::pow(RPKokeshiBlinkMgr::NUM_UNIQUE_BLINK_TIMES, BlinkGroup::NUM_BLINKS_PER_HASH)), filePath, 5) {}

    virtual ~OgBlinkFinder() = default;

    virtual std::vector<u32> inputToHashes(const BlinkGroup &input) override
    {
        return {input.toHash()};
    }

    virtual double scoreOutput(const OgBlinkFinderOutput &output, const BlinkGroup &input) override
    {
        return static_cast<f64>(BlinkGroup::compare(output.blinkGroup, input));
    }

    virtual ScoredOutput<OgBlinkFinderOutput> generatePotentialOutputFromSeed(u32 seed, const BlinkGroup &input) override
    {
        auto output = OgBlinkFinderOutput{seed, BlinkGroup(seed, input.size())};
        return {scoreOutput(output, input), output};
    }

    virtual u32 nextSeed(u32 currentSeed) override
    {
        return currentSeed + 1;
    }
};