#include "server/src/core/finders/finder_bridge.h"
#include <iostream>

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

OgWindFinderOutputWithErrorFFI find_og_wind(const OgWindFinderInputFFI &input, const OgWindFinderSettings &settings)
{
    OgWindFinderOutputWithErrorFFI finalOutput;

    try
    {

        bool ver_1_0 = settings.game == 0;

        const char *precomputeEnvVarName;
        std::string precomputeDefaultPath;

        switch (settings.game)
        {
        case 0:
            precomputeEnvVarName = "OG_WIND_PRECOMPUTE_PATH_1_0";
            precomputeDefaultPath = "/og_wind_precompute_1.0.bin";
            break;
        case 1:
            precomputeEnvVarName = "OG_WIND_PRECOMPUTE_PATH_1_1";
            precomputeDefaultPath = "/og_wind_precompute_1.1.bin";
            break;
        default:
            throw std::runtime_error("Unknown game");
        }

        const char *env = std::getenv(precomputeEnvVarName);

        const std::string filePath =
            (env && env[0] != '\0')
                ? std::string(env)
                : precomputeDefaultPath;

        OgWindFinder finder(filePath, ver_1_0);

        RPGolWindSet windSet = inputFFIToWindSet(input);

        std::vector<OgWindFinderOutput> outputs = finder.find(windSet, settings.last_known_seed, settings.num_to_check);

        for (const auto &output : outputs)
        {
            OgWindFinderOutputFFI outputFFI = windSetToOutputFFI(output.windSet);
            outputFFI.seed = output.seed;
            finalOutput.seeds.push_back(outputFFI);
        }
    }
    catch (const std::exception &e)
    {
        finalOutput.error = rust::String("An error occured");
    }
    return finalOutput;
}