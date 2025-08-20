#include "AbstractPrecomputeGenerator.h"
#include "BlinkGroup.h"

#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPKokeshiBlinkMgr.h"

#include <cmath>

class OgBlinkPrecomputeGenerator : public AbstractPrecomputeGenerator
{
public:
    OgBlinkPrecomputeGenerator() : AbstractPrecomputeGenerator(0)
    {
        this->numTempFiles = static_cast<u32>(std::pow(RPKokeshiBlinkMgr::NUM_UNIQUE_BLINK_TIMES, BlinkGroup::NUM_BLINKS_PER_HASH));
    }

    virtual ~OgBlinkPrecomputeGenerator() = default;

    virtual u32 seedToHash(u32 seed) override
    {
        return BlinkGroup(seed, BlinkGroup::NUM_BLINKS_PER_HASH).toHash();
    }
};