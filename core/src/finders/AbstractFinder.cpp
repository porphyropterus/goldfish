#include "AbstractFinder.h"

#include <fstream>
#include <stdexcept>
#include <vector>
#include <string>

#include "util_zlib.h"

template <typename TInput, typename TOutput>
AbstractFinder<TInput, TOutput>::AbstractFinder(u32 numHashes, const std::string &filePath)
    : numHashes(numHashes), filePath(filePath) {}

template <typename TInput, typename TOutput>
AbstractFinder<TInput, TOutput>::~AbstractFinder() {}

template <typename TInput, typename TOutput>
std::vector<u32> AbstractFinder<TInput, TOutput>::getSeedsFromFile(const std::string &filePath, std::vector<u32> hashes)
{
    std::vector<u32> seeds;

    if (hashes.empty())
    {
        return seeds;
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("Error: Could not open file " + filePath);
    }

    for (const auto &hash : hashes)
    {
        if (hash >= numHashes)
        {
            throw std::out_of_range("Hash " + std::to_string(hash) + " is out of range for this generator that expects " + std::to_string(numHashes) + " hashes.");
        }

        // seek `hash` number of u64s + 1 u32 for number of offsets from beginning of file
        file.seekg(hash * sizeof(u64) + sizeof(u32), std::ios::beg);

        // read the offset
        u64 offset;
        file.read(reinterpret_cast<char *>(&offset), sizeof(u64));

        // read the next offset to determine the size of the compressed data
        u64 next_offset;
        file.read(reinterpret_cast<char *>(&next_offset), sizeof(u64));

        u64 difference = next_offset - offset;

        // if there's no seeds for this hash, continue
        if (difference < sizeof(u64))
        {
            continue;
        }

        u64 size = difference - sizeof(u64);

        // seek `offset` amount of bytes ahead
        file.seekg(offset - (2 * sizeof(u64)), std::ios::cur);

        // you will now be at the location of the compressed data
        // uncompress the data
        std::vector<u8> compressedData(size);
        file.read(reinterpret_cast<char *>(compressedData.data()), size);

        std::vector<u8> decompressedData;
        decompressData(compressedData, decompressedData);

        // number of seeds is the first 4 bytes of the uncompressed data
        u32 seeds_size = 0;
        for (int i = 0; i < 4; ++i)
        {
            seeds_size |= (decompressedData[i] << (8 * i));
        }

        // construct the seeds array (the data is stored MSB->LSB)
        std::vector<u32> thisHashSeeds(seeds_size);

        int read_pos = 4;

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < seeds_size; j++)
            {
                thisHashSeeds[j] |= (decompressedData[read_pos] << (8 * (3 - i)));
                read_pos++;
            }
        }

        // un-delta encode the seeds
        for (u32 i = 1; i < seeds.size(); i++)
        {
            thisHashSeeds[i] += thisHashSeeds[i - 1];
        }

        // add thisHashSeeds to seeds
        seeds.insert(seeds.end(), thisHashSeeds.begin(), thisHashSeeds.end());
    }
    return seeds;
}

template <typename TInput, typename TOutput>
std::vector<TOutput> AbstractFinder<TInput, TOutput>::find(const TInput &input)
{
    std::vector<u32> hashes = inputToHashes(input);

    std::vector<u32> seeds = getSeedsFromFile(filePath, hashes);

    std::vector<TOutput> results;

    for (u32 seed : hashes)
    {
        TOutput output = generatePotentialOutputFromSeed(seed);
        if (doesPotentialOutputMatchInput(output, input))
        {
            results.push_back(output);
        }
    }

    return results;
}