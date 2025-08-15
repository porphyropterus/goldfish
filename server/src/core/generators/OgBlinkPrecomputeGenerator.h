#include "AbstractPrecomputeGenerator.h"

#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPKokeshiBlinkMgr.h"

#include <cmath>

class OgBlinkPrecomputeGenerator : public AbstractPrecomputeGenerator
{
public:
    OgBlinkPrecomputeGenerator(bool ver_1_0) : AbstractPrecomputeGenerator(0)
    {
        this->ver_1_0 = ver_1_0;

        this->numBlinksPerHash = ver_1_0 ? 2 : 6;
        this->numTempFiles = static_cast<u32>(std::pow(RPKokeshiBlinkMgr::NUM_UNIQUE_BLINK_TIMES, numBlinksPerHash));
    }

    virtual ~OgBlinkPrecomputeGenerator() = default;

    virtual u32 seedToHash(u32 seed) override
    {
        RPUtlRandom::initialize(seed);

        u32 hash = 0;

        for (u32 i = 0; i < numBlinksPerHash; i++)
        {
            u32 idx = RPKokeshiBlinkMgr::getRandomBlinkIndex();

            hash *= RPKokeshiBlinkMgr::NUM_UNIQUE_BLINK_TIMES;
            hash += RPKokeshiBlinkMgr::convertAbsoluteIndexToUniqueIndex(idx);
        }

        return hash;
    }

private:
    bool ver_1_0;

    u32 numBlinksPerHash;
};