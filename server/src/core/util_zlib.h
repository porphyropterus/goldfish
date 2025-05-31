#pragma once

#include <vector>
#include <stdexcept>
#include <cstdint>

#include "zlib.h"

#include "types.h"

const u8 ZLIB_HEADER[2] = {0x78, 0x9C};

// Compresses `data` into `compressedData`, removing the ZLIB header bytes.
void compressData(const std::vector<u8> &data, std::vector<u8> &compressedData);

// Decompresses `compressedData` (without ZLIB header) into `decompressedData`.
void decompressData(const std::vector<u8> &compressedData, std::vector<u8> &decompressedData);