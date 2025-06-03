#include "AbstractFinder.h"

#include "lib/RP/RPGolWindSet.h"
#include "lib/RP/RPGolDefine.h"
#include "lib/RP/RPGolConfig.h"
#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPGolDifficulty.h"

struct OgWindFinderOutput
{
    u32 seed;
    RPGolWindSet windSet;
};

class OgWindFinder : public AbstractFinder<RPGolWindSet, OgWindFinderOutput>
{
public:
    OgWindFinder(const std::string &filePath)
        : AbstractFinder<RPGolWindSet, OgWindFinderOutput>(1 << 21, filePath) {}

    virtual ~OgWindFinder() = default;

    virtual std::vector<u32> inputToHashes(const RPGolWindSet &input) override
    {
        return input.hashesWithDepth(3);
    }

    virtual OgWindFinderOutput generatePotentialOutputFromSeed(u32 seed) override
    {
        RPUtlRandom::initialize(seed);
        auto windSet = RPGolWindSet();
        RPGolConfig::getInstance()->MakeWindSet(diff_Ninehole, windSet);
        return {seed, windSet};
    }

    virtual bool doesPotentialOutputMatchInput(const OgWindFinderOutput &output, const RPGolWindSet &input) override
    {
        return output.windSet == input;
    }

    virtual u32 nextSeed(u32 currentSeed) override
    {
        RPUtlRandom::initialize(currentSeed);
        RPUtlRandom::advance(1);
        return RPUtlRandom::getSeed();
    }
};