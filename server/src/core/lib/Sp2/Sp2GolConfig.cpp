#include "lib/Sp2/Sp2GolConfig.h"
#include "lib/RP/RPGolDefine.h"
#include <iostream>

void Sp2GolConfig::MakeRandomSequence(Sp2Rand &rand, s32 arrSize, s32 max, s32 *pArray)
{
    s32 seq[22];

    for (s32 i = 0; i < max; i++)
    {
        seq[i] = -max;
    }

    for (s32 i = 0; i < max; i++)
    {

        s32 randomEmptySlot = (s32)(rand.getF32() * (max - i));

        for (s32 j = 0; j < max; j++)
        {
            if (seq[j] < 0)
            {
                if (randomEmptySlot == 0)
                {
                    seq[j] += max; // mark as selected
                    seq[i] += j;
                    break;
                }
                randomEmptySlot--;
            }
        }

        // // print pArray
        // std::cout << "seq: ";
        // for (s32 k = 0; k < arrSize; k++)
        // {
        //     std::cout << seq[k] << " ";
        // }
        // std::cout << std::endl;
    }

    for (s32 i = 0; i < arrSize; i++)
    {
        pArray[i] = seq[i];
    }
}

void Sp2GolConfig::MakePartitionedRandomArray(Sp2Rand &rand, s32 arrSize, s32 min, s32 max, s32 *pArray)
{
    const s32 MAX_GROUP_SIZE = 9;

    s32 groupSize = MAX_GROUP_SIZE;
    s32 numValues = max - min;

    if (numValues < MAX_GROUP_SIZE)
    {
        groupSize = numValues;
    }

    u32 arrIdx = 0;

    while (arrIdx < arrSize)
    {

        // make partial group if needed
        if (arrSize - arrIdx < groupSize)
        {
            groupSize = arrSize - arrIdx;
        }

        MakeRandomSequence(rand, groupSize, numValues, &pArray[arrIdx]);

        arrIdx += groupSize;
    }

    // add min value to everything
    for (s32 i = 0; i < arrSize; i++)
    {
        pArray[i] += min;
    }
}

void Sp2GolConfig::MakeWindSet(Sp2Rand &rand, const RPGolDifficulty &diff, RPGolWindSet &out)
{
    int dirs[21];
    int speeds[21];

    MakePartitionedRandomArray(rand, 21, 0, RPGolDefine::MAX_WIND_DIR, dirs);
    MakePartitionedRandomArray(rand, 21, diff.minWind, diff.maxWind + 1, speeds);

    for (int i = 0; i < 21; i++)
    {
        // if the hole is outside the difficulty range, set to wildcard
        if (i < diff.startHole || i > diff.endHole)
        {
            out[i].mDirection = RPGolDefine::MAX_WIND_DIR;
            out[i].mSpeed = RPGolDefine::MAX_WIND_SPD;
        }
        else
        {
            out[i].mDirection = dirs[i];
            out[i].mSpeed = speeds[i];
        }
    }
}