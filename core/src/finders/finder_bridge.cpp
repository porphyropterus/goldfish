#include "finder_bridge.h"

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