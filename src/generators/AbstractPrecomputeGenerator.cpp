#include <sstream>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <chrono>

#include <vector>
#include <unordered_map>
#include <algorithm>

#include "types.h"
#include "lib/RP/RPUtlRandom.h"
#include "lib/RP/RPGolConfig.h"
#include "lib/RP/RPGolWindSet.h"
#include "lib/RP/RPGolDifficulty.h"
#include "lib/rvl/OSTime.h"

#include "util_zlib.h"

#include "AbstractPrecomputeGenerator.h"

AbstractPrecomputeGenerator::AbstractPrecomputeGenerator(u32 numTempFiles)
    : numTempFiles(numTempFiles), currentSeed(0) {}

AbstractPrecomputeGenerator::~AbstractPrecomputeGenerator()
{
    for (u32 i = 0; i < numTempFiles; i++)
    {
        if (hashToSeeds.count(i) && hashToSeeds[i].file && hashToSeeds[i].file.is_open())
        {
            hashToSeeds[i].file.close();
        }
    }
}

u32 AbstractPrecomputeGenerator::nextSeed(u32 currentSeed)
{
    return currentSeed + 1;
}

u32 AbstractPrecomputeGenerator::seedToHash(u32 seed)
{
    // default behavior
    return seed % numTempFiles;
}

void AbstractPrecomputeGenerator::generateTempFiles()
{
    std::cout << "Generating temp files" << std::endl;

    // create folder called temp if it doesn't exist
    if (!std::filesystem::exists("./temp"))
    {
        std::filesystem::create_directory("./temp");
    }

    // initialize hashmap
    for (u32 i = 0; i < numTempFiles; i++)
    {
        hashToSeeds[i] = TempFileAndSeeds();
        std::string filename = "./temp/" + std::to_string(i) + ".bin";
        // if file exists, clear it; otherwise, create it
        hashToSeeds[i].file.open(filename, std::ios::binary | std::ios::trunc);
        if (!hashToSeeds[i].file)
        {
            std::cerr << "Error: Could not open file " << i << ".bin" << std::endl;
            exit(1);
        }
    }
}

void AbstractPrecomputeGenerator::fillTempFiles()
{
    std::cout << "Filling temp files" << std::endl;

    for (u32 i = 0; i < 0x8; i++)
    {
        for (u32 j = 0; j < 0x10000; j++)
        {
            // add seed to corresponding file
            // std::cout << seedToHash(currentSeed) << " for seed: " << currentSeed << std::endl;
            hashToSeeds[seedToHash(currentSeed)].seeds.push_back(currentSeed);
            currentSeed = nextSeed(currentSeed);
        }

        // write to files
        for (u32 h = 0; h < numTempFiles; h++)
        {
            if (!hashToSeeds[h].seeds.empty())
            {
                hashToSeeds[h].file.write(reinterpret_cast<const char *>(hashToSeeds[h].seeds.data()), hashToSeeds[h].seeds.size() * sizeof(u32));
                hashToSeeds[h].seeds.clear();
            }
        }

        std::cout << "0x" << std::hex << i << "/0x10" << std::endl;
        // close all files
    }

    std::cout << "closing temp files" << std::endl;
    for (u32 h = 0; h < numTempFiles; h++)
    {
        if (hashToSeeds[h].file.is_open())
        {
            hashToSeeds[h].file.close();
        }
    }
}

void AbstractPrecomputeGenerator::generateFinalFileFromTempFiles(const std::string &finalFilePath)
{
    std::cout << "Generating final file from temp files" << std::endl;

    // for all files in the temp folder
    // rewrite them so the most significant bits are first, then the 2nd most, etc.

    std::vector<u32> seeds;
    std::vector<u8> new_bytes;
    u32 read_seed;

    std::ofstream output_file(finalFilePath, std::ios::binary | std::ios::trunc);

    // stores number of bytes forward you would need to go to get the corresponding hash
    // fill with 0s for now
    std::vector<u64> offsets(numTempFiles, 0);
    // except for the first offset would be 1 << 14 * sizeof(u64)
    offsets[0] = (numTempFiles) * sizeof(u64);

    // write the number of offsets to the file

    u32 numOffsets = numTempFiles;
    output_file.write(reinterpret_cast<const char *>(&numOffsets), sizeof(u32));

    // write placeholder offsets to the file
    for (u32 i = 0; i < numTempFiles; i++)
    {
        output_file.write(reinterpret_cast<const char *>(&offsets[i]), sizeof(u64));
    }

    int zlib_counter = 0;

    // create hex string for progress bar of compressor

    std::stringstream ss;
    ss << "/0x" << std::hex << numTempFiles;
    std::string compressorProgressBarTotal = ss.str();
    if (compressorProgressBarTotal.length() >= 2)
    {
        compressorProgressBarTotal.erase(compressorProgressBarTotal.length() - 2);
    }

    for (int i = 0; i < numTempFiles; i++)
    {

        seeds.clear();
        new_bytes.clear();

        // read from ith file, put contents into seeds
        std::ifstream file = std::ifstream("./temp/" + std::to_string(i) + ".bin", std::ios::binary);

        if (!file)
        {
            std::cout << "Error: Could not open file " << std::to_string(i) << ".bin" << std::endl;
        }
        else
        {
            while (file.read(reinterpret_cast<char *>(&read_seed), sizeof(u32)))
            {
                std::cout << "Read seed: " << std::dec << read_seed << std::endl;
                seeds.push_back(read_seed);
            }
        }

        if (seeds.size() > 0)
        {
            std::cout << "Seeds size: " << seeds.size() << std::endl;
        }

        // sort the seeds
        std::sort(seeds.begin(), seeds.end());

        // delta encode the seeds now that they are sorted
        if (seeds.size() > 1)
        {
            for (u32 i = seeds.size() - 1; i > 0; i--)
            {
                seeds[i] -= seeds[i - 1];
            }
        }

        // add size to beginning of the new bytes
        u32 size = seeds.size();

        for (int i = 0; i < 4; ++i)
        {
            new_bytes.push_back((size >> (8 * i)) & 0xFF);
        }

        // convert the seeds to msb -> lsb
        for (u32 byte_index = 0; byte_index < 4; ++byte_index)
        {
            for (const auto &seed : seeds)
            {
                u8 byte = (seed >> (8 * (3 - byte_index))) & 0xFF;
                new_bytes.push_back(byte);
            }
        }

        if (new_bytes.size() > 4)
            std::cout << "New bytes size: " << new_bytes.size() << std::endl;

        // // close input file
        file.close();
        // // i can delete the temp file now
        // std::filesystem::remove("./temp/" + std::to_string(i) + ".bin");

        // compress the new bytes
        std::vector<u8> compressedData;
        compressData(new_bytes, compressedData);
        // write the compressed data to the file
        output_file.write(reinterpret_cast<const char *>(compressedData.data()), compressedData.size());

        // determine next offset
        if (i != (numTempFiles)-1)
            offsets[i + 1] = offsets[i] + compressedData.size() - sizeof(u64);

        if (i % 0x100 == 0)
        {
            zlib_counter++;
            std::cout << "0x" << std::hex << zlib_counter << compressorProgressBarTotal << std::endl;
        }
    }

    // write actual offsets to the file
    output_file.seekp(sizeof(u32), std::ios::beg);
    output_file.write(reinterpret_cast<const char *>(offsets.data()), offsets.size() * (sizeof(u64)));
}

void AbstractPrecomputeGenerator::generateFile(const std::string &filePath)
{
    generateTempFiles();
    fillTempFiles();
    generateFinalFileFromTempFiles(filePath);
}

// int main()
// {
//     Generator generator;
//     auto start = std::chrono::high_resolution_clock::now();
//     generator.dumpSeeds();
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> elapsed = end - start;
//     std::cout << "Time per seed: " << elapsed.count() / (0x800 * 65536) << "s" << std::endl;

//     // RPGolWindSet wind;

//     // RPGolConfig *pInstance = RPGolConfig::getInstance();
//     // pInstance->MakeWindSet(diff_Ninehole, wind);

//     // char buf[256];
//     // wind.toString(buf, "{", "}", "[", "]", true);
//     // std::cout << buf << std::endl;

//     // // open file as an int pointer and start reading it
//     // std::ifstream file("./seeds/0/1.bin", std::ios::binary);
//     // u32 seed;
//     // int count;
//     // RPGolWindSet wind;
//     // auto start = std::chrono::high_resolution_clock::now();
//     // while (file.read(reinterpret_cast<char *>(&seed), sizeof(u32)))
//     // {
//     //     count++;
//     //     std::cout << "Seed: " << seed << std::endl;
//     //     getWindSetFromSeed(seed, wind);
//     // }
//     // auto end = std::chrono::high_resolution_clock::now();

//     // std::cout << "Time per seed: " << std::chrono::duration<double>(end - start).count() / count << "s" << std::endl;
//     // std::cout << "Total time: " << std::chrono::duration<double>(end - start).count() << "s" << std::endl;

//     return 0;
// }