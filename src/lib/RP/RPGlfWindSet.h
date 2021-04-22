#pragma once
#include <lib/RP/RPGlfDefine.h>
#include <string>

typedef u32 Score_t;

struct Wind
{
    u32 mDirection;
    s32 mSpeed;

    inline Wind& operator=(const Wind& rhs)
    {
        mDirection = rhs.mDirection;
        mSpeed = rhs.mSpeed;

        return *this;
    }
};

struct RPGlfWindSet
{
    RPGlfWindSet() : mWinds() {}
    Wind mWinds[RPGlfDefine::HOLE_SIZE];

    Score_t scoreAgainstTarget(const RPGlfWindSet&);
    std::string toString(std::string startDelim = "{", std::string endDelim = "}") const;
    RPGlfWindSet& operator=(const RPGlfWindSet& rhs);
};

const Score_t scoreBase = 8;
const Score_t perfectScore = (scoreBase * 2) * RPGlfDefine::HOLE_SIZE;