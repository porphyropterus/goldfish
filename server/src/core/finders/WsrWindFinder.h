#include "ExactMatchFinder.h"

#include "lib/RP/RPGolWindSet.h"
#include "lib/RP/RPGolDefine.h"
#include "lib/RP/RPGolDifficulty.h"

#include "lib/Sp2/Sp2Rand.h"
#include "lib/Sp2/Sp2GolConfig.h"

#include "OgWindFinder.h"

class WsrWindFinder : public ExactMatchFinder<RPGolWindSet, OgWindFinderOutput>
{
public:
    WsrWindFinder(const std::string &filePath)
        : ExactMatchFinder<RPGolWindSet, OgWindFinderOutput>(1 << 21, filePath) {}

    virtual ~WsrWindFinder() = default;

    virtual std::vector<u32> inputToHashes(const RPGolWindSet &input) override
    {
        return input.hashesWithDepth(3);
    }

    virtual OgWindFinderOutput generatePotentialOutputFromSeed(u32 seed) override
    {
        rand.initialize(seed);
        auto windSet = RPGolWindSet(21);
        Sp2GolConfig::getInstance()->MakeWindSet(rand, diff_Sp2_18Hole, windSet);
        return {seed, windSet};
    }

    virtual bool doesPotentialOutputMatchInput(const OgWindFinderOutput &output, const RPGolWindSet &input) override
    {
        return output.windSet == input;
    }

    virtual u32 nextSeed(u32 currentSeed) override
    {
        rand.initialize(currentSeed);
        return rand.Rand();
    }

private:
    Sp2Rand rand;
};