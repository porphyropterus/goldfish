#pragma once

#ifndef RP_SPORTS_GOL_CONFIG_H
#define RP_SPORTS_GOL_CONFIG_H
#include "types.h"

struct RPGolDifficulty;
class RPGolWindSet;

class RPGolConfig
{
public:
    static void MakeRandomSequence(s32 max, s32 *pArray, bool ver_1_0);                    // at 8029dea8
    static void MakeWindSet(const RPGolDifficulty &diff, RPGolWindSet &out, bool ver_1_0); // based on 8029dcf4

    static RPGolConfig *getInstance()
    {
        static RPGolConfig instance;
        return &instance;
    }

private:
    RPGolConfig() {}
    ~RPGolConfig() {}

    // Number of RNG calls between boot and wind generation in-game.
    static constexpr u32 CALC_BEFORE_WIND = 9;
};
#endif