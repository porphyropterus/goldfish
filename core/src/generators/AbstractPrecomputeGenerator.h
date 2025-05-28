#pragma once

#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>

#include "types.h"

class AbstractPrecomputeGenerator
{
protected:
    class TempFileAndSeeds
    {
    public:
        std::ofstream file;
        std::vector<u32> seeds;
    };

    std::unordered_map<u32, TempFileAndSeeds> hashToSeeds;
    u32 numTempFiles;
    u32 currentSeed = 0;

public:
    AbstractPrecomputeGenerator(u32 numTempFiles);
    ~AbstractPrecomputeGenerator();

    // To make a new generator, extend this class and create a mapping from seeds to groups
    virtual u32 seedToHash(u32 seed);

    // For if you want to do every seed in order numerically, or in order of lcg, or whatever
    virtual u32 nextSeed(u32 currentSeed);

    void generateTempFiles();
    void fillTempFiles();
    void generateFinalFileFromTempFiles(const std::string &finalFilePath);
    void generateFile(const std::string &filePath);
};