#pragma once
#include "types.h"
#include <lib/RP/RPGlfDefine.h>
#include <lib/RP/RPGlfWindSet.h>
#include "DifficultyInfo.h"
#include <string>

class RPGlfConfig
{
public:
    void makeRandomSequence(s32, s32*);        // at 8029dea8
    void chooseWindSet(); // at 8029dcf4
    
    RPGlfWindSet& getWindSet();
    Wind& getWind(u32);

    void setWind(u32, const Wind&);
    void setDifficulty(const DifficultyInfo&);

    DifficultyInfo& getDifficulty();

    static RPGlfConfig* getInstance();

private:
    inline RPGlfConfig() : mWindSet() {}
    RPGlfWindSet mWindSet;
    DifficultyInfo mDiff;

    static RPGlfConfig* mInstance;
};

// Array sizes for makeRandomSequence
const u32 randomSpeedArraySize = 16;
const u32 randomDirArraySize = 8;

// Number of RNG calls before makeWindSet.
// Ex: seed must be advanced 9 times between its instantiation
// and makeWindSet being called to be accurate
const u32 CALC_BEFORE_WIND = 9;

const std::string windDirStrings[] = {
    "S", "SE", "E", "NE", "N", "NW", "W", "SW"
};

const std::string wildCard = "*";

RPGlfDefine::WindDir stringToDir(std::string&);
