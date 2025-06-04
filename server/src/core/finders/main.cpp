#include "OgWindFinder.h"
#include "WindArgParser.h"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <precompute_file> <wind_string>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    std::string windStr = argv[2];

    RPGolWindSet windSet;
    if (!WindArgParser::parseTargetWindSet(windStr, windSet))
    {
        std::cerr << "Error: Invalid wind argument" << std::endl;
        return 1;
    }

    OgWindFinder finder(filePath, false);
    auto results = finder.find(windSet);

    char buffer[1024] = {0};

    for (const auto &result : results)
    {
        std::cout << "Seed: 0x" << std::hex << result.seed << std::endl;
        result.windSet.toString(buffer);
        std::cout << "Wind: " << buffer << "\n\n";
    }

    return 0;
}