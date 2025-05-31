#include "util_zlib.h"

void compressData(const std::vector<u8> &data, std::vector<u8> &compressedData)
{
    uLongf compressedSize = compressBound(data.size());
    compressedData.resize(compressedSize);

    if (compress(compressedData.data(), &compressedSize, data.data(), data.size()) != Z_OK)
    {
        throw std::runtime_error("Compression failed");
    }

    compressedData.resize(compressedSize);

    // remove the first 2 bytes (ZLIB header) from compressedData
    if (compressedData.size() > 2)
    {
        compressedData.erase(compressedData.begin(), compressedData.begin() + 2);
    }
}

void decompressData(const std::vector<u8> &compressedData, std::vector<u8> &decompressedData)
{
    // Add ZLIB header back
    std::vector<u8> dataWithHeader;
    dataWithHeader.reserve(2 + compressedData.size());
    dataWithHeader.push_back(ZLIB_HEADER[0]);
    dataWithHeader.push_back(ZLIB_HEADER[1]);
    dataWithHeader.insert(dataWithHeader.end(), compressedData.begin(), compressedData.end());

    uLongf decompressedSize = dataWithHeader.size() * 4; // Initial guess for decompressed size
    decompressedData.resize(decompressedSize);

    while (uncompress(decompressedData.data(), &decompressedSize, dataWithHeader.data(), dataWithHeader.size()) == Z_BUF_ERROR)
    {
        decompressedSize *= 2; // Double the size and try again
        decompressedData.resize(decompressedSize);
    }

    decompressedData.resize(decompressedSize);
}