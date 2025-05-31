#include "finder_bridge.h"

RPGolWindSet inputFFIToWindSet(const OgWindFinderInputFFI &input)
{
    RPGolWindSet windSet;
    for (size_t i = 0; i < RPGolDefine::HOLE_SIZE; i++)
    {
        windSet.mWinds[i] = input.winds[i];
    }
    return windSet;
}

OgWindFinderOutputFFI windSetToOutputFFI(const RPGolWindSet &windSet)
{
    OgWindFinderOutputFFI output;

    output.winds.resize(RPGolDefine::HOLE_SIZE);
    for (u32 i = 0; i < RPGolDefine::HOLE_SIZE; i++)
    {
        output.winds[i] = windSet.mWinds[i];
    }

    return output;
}

std::vector<OgWindFinderOutputFFI> find_og_wind(OgWindFinderInputFFI &input)
{
    OgWindFinder finder("~/og_wind_precompute.bin");
    RPGolWindSet windSet = inputFFIToWindSet(input);

    std::vector<OgWindFinderOutput> outputs = finder.find(windSet);

    std::vector<OgWindFinderOutputFFI> outputFFIs;
    for (const auto &output : outputs)
    {
        OgWindFinderOutputFFI outputFFI = windSetToOutputFFI(output.windSet);
        outputFFI.seed = output.seed;
        outputFFIs.push_back(outputFFI);
    }

    return outputFFIs;
}