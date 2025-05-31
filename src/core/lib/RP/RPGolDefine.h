#pragma once

#ifndef RP_SPORTS_GOL_DEFINE_H
#define RP_SPORTS_GOL_DEFINE_H
#include "types.h"
#include <cstring>
#include <cctype>

namespace RPGolDefine
{
    enum WindDir
    {
        /* 0x0000 */ SOUTH,
        /* 0x0001 */ SOUTHEAST,
        /* 0x0002 */ EAST,
        /* 0x0003 */ NORTHEAST,
        /* 0x0004 */ NORTH,
        /* 0x0005 */ NORTHWEST,
        /* 0x0006 */ WEST,
        /* 0x0007 */ SOUTHWEST,
        /* 0x0008 */ MAX_WIND_DIR,

        /* 0x0009 */ WILDCARD_DIR
    };

    enum WindSpd
    {
        /* 0x000F */ MAX_WIND_SPD = 16,

        /* 0x0010 */ WILDCARD_SPD = 17
    };

    enum Gamemode
    {
        /* 0x0000 */ GOL_COURSE_SELECT,

        /* 0x0001 */ GOL_TR_ONEPAT,
        /* 0x0002 */ GOL_TR_NEARPIN,
        /* 0x0003 */ GOL_TR_TARGET,

        /* 0x0004 */ GOL_FT_ONEPAT,
        /* 0x0005 */ GOL_FT_NEARPIN,
        /* 0x0006 */ GOL_FT_TARGET,

        /* 0x0007 */ GOL_NINEHOLE,

        /* 0x0008 */ GOL_3H_BEGINNER,
        /* 0x0009 */ GOL_3H_INTERMED,
        /* 0x000A */ GOL_3H_EXPERT,
    };

    constexpr u32 HOLE_SIZE = 9;
    constexpr u32 PLAYER_SIZE = 4;

    /// <summary>
    /// Converts a direction string to a WindDir enum value
    /// </summary>
    /// <param name="s">Direction string</param>
    /// <returns>WindDir enum value</returns>
    static u32 StringToDir(const char *dir)
    {
        // String should be at most 2 chars long
        u32 len = std::strlen(dir);
        if (len > 2)
            return MAX_WIND_DIR;

        switch (len)
        {
        case 1:
            switch (std::toupper(dir[0]))
            {
            case 'N':
                return NORTH;
            case 'S':
                return SOUTH;
            case 'E':
                return EAST;
            case 'W':
                return WEST;
            case '*':
                return WILDCARD_DIR;
            default:
                return MAX_WIND_DIR;
            }

        case 2:
            if (strcmp(dir, "NE") == 0)
                return NORTHEAST;
            if (strcmp(dir, "NW") == 0)
                return NORTHWEST;
            if (strcmp(dir, "SE") == 0)
                return SOUTHEAST;
            if (strcmp(dir, "SW") == 0)
                return SOUTHWEST;
            return MAX_WIND_DIR;
        }

        return MAX_WIND_DIR;
    }

    static const char *DirToString(u32 dir)
    {
        if (dir == WILDCARD_DIR)
            return "*";

        if (dir >= MAX_WIND_DIR)
        {
            assert(false && "Invalid wind direction!");
            return "INVALID";
        }

        constexpr const char *windDirStrings[] = {
            "S", "SE", "E", "NE", "N", "NW", "W", "SW"};

        return windDirStrings[dir];
    }
}
#endif