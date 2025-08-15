#include "AbstractPrecomputeGenerator.h"

#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPKokeshiBlinkMgr.h"

#include <cmath>

class OgBlinkPrecomputeGenerator : public AbstractPrecomputeGenerator
{
public:
    OgBlinkPrecomputeGenerator() : AbstractPrecomputeGenerator(0)
    {
        this->numTempFiles = static_cast<u32>(std::pow(RPKokeshiBlinkMgr::NUM_UNIQUE_BLINK_TIMES, NUM_BLINKS_PER_HASH));
    }

    virtual ~OgBlinkPrecomputeGenerator() = default;

    virtual u32 seedToHash(u32 seed) override
    {
        RPUtlRandom::initialize(seed);

        u32 hash = 0;

        for (u32 i = 0; i < NUM_BLINKS_PER_HASH; i++)
        {
            u32 idx = RPKokeshiBlinkMgr::getRandomBlinkIndex();

            hash *= RPKokeshiBlinkMgr::NUM_UNIQUE_BLINK_TIMES;
            hash += RPKokeshiBlinkMgr::convertAbsoluteIndexToUniqueIndex(idx);
        }

        return hash;
    }

private:
    static constexpr u32 NUM_BLINKS_PER_HASH = 6;
};