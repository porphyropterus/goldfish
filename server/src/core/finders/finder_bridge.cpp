#include "server/src/core/finders/finder_bridge.h"

RPGolWindSet inputFFIToWindSet(const OgWindFinderInputFFI &input)
{
    RPGolWindSet windSet;
    for (size_t i = 0; i < RPGolDefine::HOLE_SIZE; i++)
    {
        windSet.mWinds[i].mDirection = input.winds[i].mDirection;
        windSet.mWinds[i].mSpeed = input.winds[i].mSpeed;
    }
    return windSet;
}

OgWindFinderOutputFFI windSetToOutputFFI(const RPGolWindSet &windSet)
{
    OgWindFinderOutputFFI output;

    for (u32 i = 0; i < RPGolDefine::HOLE_SIZE; i++)
    {
        output.winds[i].mDirection = windSet.mWinds[i].mDirection;
        output.winds[i].mSpeed = windSet.mWinds[i].mSpeed;
    }

    return output;
}

const std::vector<OgWindFinderOutputFFI> &find_og_wind(const OgWindFinderInputFFI &input)
{
    OgWindFinder finder("/home/vince/og_wind_precompute.bin");
    RPGolWindSet windSet = inputFFIToWindSet(input);

    std::vector<OgWindFinderOutput> outputs = finder.find(windSet);

    static std::vector<OgWindFinderOutputFFI> outputFFIs;
    for (const auto &output : outputs)
    {
        OgWindFinderOutputFFI outputFFI = windSetToOutputFFI(output.windSet);
        outputFFI.seed = output.seed;
        outputFFIs.push_back(outputFFI);
    }

    return outputFFIs;
}