#pragma once

#include "lib/RP/RPGolDefine.h"
#include "lib/RP/RPGolWindSet.h"

#include "OgWindFinder.h"

#include "types.h"

struct OgWindFinderInputFFI
{
    std::vector<Wind> winds;
};

struct OgWindFinderOutputFFI
{
    u32 seed;
    std::vector<Wind> winds;
};

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

std::vector<OgWindFinderOutputFFI> find_og_wind(OgWindFinderInputFFI &input);