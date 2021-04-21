#pragma once
#include "types.h"

struct DifficultyInfo
{
    u32 startHole, endHole;
    s32 minWind, maxWind;

    inline DifficultyInfo& operator=(const DifficultyInfo& rhs)
    {
        startHole = rhs.startHole;
        endHole = rhs.endHole;
        minWind = rhs.minWind;
        maxWind = rhs.maxWind;

        return *this;
    }
};

const DifficultyInfo diff_Beginner = { 0, 2, 0, 5 };
const DifficultyInfo diff_Intermediate = { 3, 5, 5, 10 };
const DifficultyInfo diff_Expert = { 6, 8, 10, 16 };
const DifficultyInfo diff_Ninehole = { 0, 8, 0, 15 };
