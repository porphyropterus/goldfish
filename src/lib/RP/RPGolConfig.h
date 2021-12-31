#pragma once

#ifndef RP_SPORTS_GOL_CONFIG_H
#define RP_SPORTS_GOL_CONFIG_H
#include "types.h"

struct RPGolDifficulty;
class RPGolWindSet;

class RPGolConfig
{
public:
    static void MakeRandomSequence(s32 max, s32* pArray); // at 8029dea8
    static void MakeWindSet(const RPGolDifficulty& diff, RPGolWindSet& out); // based on 8029dcf4

private:
    RPGolConfig() {}
    ~RPGolConfig() {}

    // Number of RNG calls between boot and wind generation in-game.
    static constexpr u32 CALC_BEFORE_WIND = 9;
};
#endif