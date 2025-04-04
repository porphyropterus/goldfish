#include <iostream>
#include <fstream>
#include <vector>

#include "zlib.h"

#include "types.h"

void decompressData(const std::vector<u8> &compressedData, std::vector<u8> &decompressedData)
{
    uLongf decompressedSize = compressedData.size() * 4; // Initial guess for decompressed size
    decompressedData.resize(decompressedSize);

    while (uncompress(decompressedData.data(), &decompressedSize, compressedData.data(), compressedData.size()) == Z_BUF_ERROR)
    {
        decompressedSize *= 2; // Double the size and try again
        decompressedData.resize(decompressedSize);
    }

    decompressedData.resize(decompressedSize);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    auto file_path = argv[1];
    std::ifstream file(file_path, std::ios::binary);
    if (!file)
    {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    u32 num_offsets;
    file.read(reinterpret_cast<char *>(&num_offsets), sizeof(u32));

    // seek 0 offsets ahead
    // file.seekg(sizeof(u64) * 0, std::ios::cur);
    // read the offset
    u64 offset;
    file.read(reinterpret_cast<char *>(&offset), sizeof(u64));

    std::cout << "Offset: " << offset << std::endl;

    // read the next offset to determine the size of the data
    u64 next_offset;
    file.read(reinterpret_cast<char *>(&next_offset), sizeof(u64));

    std::cout << "Next offset: " << next_offset << std::endl;

    // calculate the size of the data
    u64 size = next_offset - offset - sizeof(u64);

    // get the next offset to determine the next size
    // print offset
    std::cout << "Size: " << size << std::endl;

    // seek offset amount of bytes ahead
    file.seekg(offset - (2 * sizeof(u64)), std::ios::cur);

    // uncompress the data
    std::vector<u8> compressedData(size);
    file.read(reinterpret_cast<char *>(compressedData.data()), size);

    std::vector<u8> decompressedData;
    decompressData(compressedData, decompressedData);

    // construct seeds array

    // size is the first 4 bytes
    u32 seeds_size = 0;
    for (int i = 0; i < 4; ++i)
    {
        seeds_size |= (decompressedData[i] << (8 * (3 - i)));
    }

    // construct the seeds array (the data is stored MSB->LSB)
    std::vector<u32> seeds(seeds_size);

    for (u32 i = 0; i < seeds_size; ++i)
    {
        seeds[i] = 0;
        for (int j = 0; j < 4; ++j)
        {
            seeds[i] |= (decompressedData[i * 4 + j] << (8 * (3 - j)));
        }
    }

    // print all seeds
    for (u32 i = 0; i < seeds_size; ++i)
    {
        std::cout << seeds[i] << std::endl;
    }
}