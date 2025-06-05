#include "AbstractPrecomputeGenerator.h"

#include "lib/RP/RPGolDifficulty.h"
#include "lib/RP/RPGolConfig.h"
#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPGolWindSet.h"

class OgWindPrecomputeGenerator : public AbstractPrecomputeGenerator
{
public:
    OgWindPrecomputeGenerator() : AbstractPrecomputeGenerator(1 << 21) {}

    virtual ~OgWindPrecomputeGenerator() = default;

    virtual u32 seedToHash(u32 seed) override
    {
        auto windSet = RPGolWindSet();
        RPUtlRandom::initialize(seed);
        RPGolConfig::getInstance()->MakeWindSet(diff_Ninehole, windSet, true);
        return windSet.hashesWithDepth(3)[0];
    }

    virtual u32 nextSeed(u32 currentSeed) override
    {
        return currentSeed + 1;
    }
};