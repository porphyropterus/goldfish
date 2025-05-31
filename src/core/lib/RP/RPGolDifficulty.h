#pragma once

#ifndef RP_SPORTS_GOL_DIFFICULTY_H
#define RP_SPORTS_GOL_DIFFICULTY_H
#include "types.h"

struct RPGolDifficulty
{
    u32 startHole;
    u32 endHole;
    s32 minWind; // m/s
    s32 maxWind; // m/s
};

static constexpr RPGolDifficulty diff_Beginner =     { 0, 2,  0,  5 };
static constexpr RPGolDifficulty diff_Intermediate = { 3, 5,  5, 10 };
static constexpr RPGolDifficulty diff_Expert =       { 6, 8, 10, 16 };
static constexpr RPGolDifficulty diff_Ninehole =     { 0, 8,  0, 15 };
#endif