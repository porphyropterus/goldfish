#include "lib/Sp2/Sp2GolConfig.h"
#include "lib/RP/RPGolDefine.h"
#include <iostream>

void Sp2GolConfig::MakeRandomSequence(Sp2Rand &rand, s32 arrSize, s32 max, s32 *pArray)
{
    // exact same as og, but the array size doesn't have to be the max value
    for (s32 i = 0; i < arrSize; i++)
        pArray[i] = -max;

    s32 slotsFilled = 0;
    while (true)
    {
        s32 random = (s32)(rand.getF32() * (max - slotsFilled));

        if (max > 0)
        {
            for (s32 i = 0, j = 0; i < arrSize; i++)
            {
                if ((pArray[j] < 0) && (--random < 0))
                {
                    pArray[j] += max;
                    pArray[slotsFilled++] += j;

                    if (slotsFilled >= arrSize)
                        return;

                    break;
                }
                else
                    j++;

                // for (int x = 0; x < arrSize; x++)
                // {
                //     std::cout << pArray[x] << " ";
                // }
                // std::cout << std::endl;
            }
        }
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