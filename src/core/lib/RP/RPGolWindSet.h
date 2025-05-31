#pragma once

#ifndef RP_SPORTS_GOL_WIND_SET_H
#define RP_SPORTS_GOL_WIND_SET_H
#include "types.h"

#include "lib/RP/RPGolDefine.h"

#include <string>
#include <vector>

typedef u32 Score_t;

struct Wind
{
    u32 mDirection;
    s32 mSpeed;
};

class RPGolWindSet
{
public:
    RPGolWindSet() : mWinds() {}

    bool operator==(const RPGolWindSet &rhs) const
    {
        for (u32 i = 0; i < RPGolDefine::HOLE_SIZE; i++)
        {
            // if any direction or speed is not equal, return false
            // if either are wildcard, they count as matching
            if ((mWinds[i].mDirection != rhs.mWinds[i].mDirection && mWinds[i].mDirection != RPGolDefine::WILDCARD_DIR && rhs.mWinds[i].mDirection != RPGolDefine::WILDCARD_DIR) ||
                (mWinds[i].mSpeed != rhs.mWinds[i].mSpeed && mWinds[i].mSpeed != RPGolDefine::WILDCARD_SPD && rhs.mWinds[i].mSpeed != RPGolDefine::WILDCARD_SPD))
            {
                return false;
            }
        }
        return true;
    }
    Wind &operator[](int i) { return mWinds[i]; }

    std::vector<unsigned int> hashesWithDepth(int depth) const;
    Score_t scoreAgainst(const RPGolWindSet &) const;
    void toString(char *buf, const char *setStartDelim = "{", const char *setEndDelim = "}",
                  const char *termStartDelim = "", const char *termEndDelim = ", ", bool bCloseEndDelim = false) const;

    Wind mWinds[RPGolDefine::HOLE_SIZE];

private:
    static constexpr Score_t scoreBase = 8;
    static constexpr Score_t perfectScore = (scoreBase * 2) * RPGolDefine::HOLE_SIZE;
};
#endif