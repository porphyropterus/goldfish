#pragma once

#include "types.h"

#include "lib/RP/RPGolDifficulty.h"
#include "lib/RP/RPGolWindSet.h"
#include "lib/Sp2/Sp2Rand.h"

class Sp2GolConfig
{
public:
    static Sp2GolConfig *getInstance()
    {
        static Sp2GolConfig instance;
        return &instance;
    }

    static void MakeRandomSequence(Sp2Rand &rand, s32 arrSize, s32 max, s32 *pArray);
    static void MakePartitionedRandomArray(Sp2Rand &rand, s32 arrSize, s32 min, s32 max, s32 *pArray);
    static void MakeWindSet(Sp2Rand &rand, const RPGolDifficulty &diff, RPGolWindSet &out);

private:
    Sp2GolConfig() {}
    ~Sp2GolConfig() {}
};