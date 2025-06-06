#include "AbstractPrecomputeGenerator.h"

#include "lib/RP/RPGolDifficulty.h"
#include "lib/RP/RPGolWindSet.h"

#include "lib/Sp2/Sp2GolConfig.h"
#include "lib/Sp2/Sp2Rand.h"

class WsrWindPrecomputeGenerator : public AbstractPrecomputeGenerator
{
public:
    WsrWindPrecomputeGenerator() : AbstractPrecomputeGenerator(1 << 21) {}

    virtual ~WsrWindPrecomputeGenerator() = default;

    virtual u32 seedToHash(u32 seed) override
    {
        auto windSet = RPGolWindSet(21);
        rand.initialize(seed);
        Sp2GolConfig::MakeWindSet(rand, diff_Sp2_18Hole, windSet);
        return windSet.hashesWithDepth(3)[0];
    }

    virtual u32 nextSeed(u32 currentSeed) override
    {
        return currentSeed + 1;
    }

private:
    Sp2Rand rand;
};