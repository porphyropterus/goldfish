#pragma once
#include "types.h"

namespace RPGlfDefine
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
        /* 0x0008 */ MAX_WIND_DIV
	};

    const u32 HOLE_SIZE = 9;
    const u32 PLAYER_SIZE = 4;
    const u32 WIND_SPEED_SIZE = 15; // m/s
}
