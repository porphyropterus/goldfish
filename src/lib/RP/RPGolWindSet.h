#pragma once

#ifndef RP_SPORTS_GOL_WIND_SET_H
#define RP_SPORTS_GOL_WIND_SET_H
#include "types.h"
#include "RPGolDefine.h"
#include <string>

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

    Wind &operator[](int i) { return mWinds[i]; }

    Score_t scoreAgainst(const RPGolWindSet &) const;
    void toString(char *buf, const char *setStartDelim = "{", const char *setEndDelim = "}",
                  const char *termStartDelim = "", const char *termEndDelim = ", ", bool bCloseEndDelim = false) const;

    Wind mWinds[RPGolDefine::HOLE_SIZE];

private:
    static constexpr Score_t scoreBase = 8;
    static constexpr Score_t perfectScore = (scoreBase * 2) * RPGolDefine::HOLE_SIZE;
};
#endif