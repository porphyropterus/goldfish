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
        /* 0x0008 */ MAX_WIND_DIV,
		// For command-line use
		/* 0x0009 */ WILDCARD
	};

	enum WindSpd
	{
		/* 0x000F */ MAX_WIND_SPD = 15,
		// For command-line use
		/* 0x0010 */ WILDCARD = 16
	};

    const u32 HOLE_SIZE = 9;
    const u32 PLAYER_SIZE = 4;

	enum RPGolGamemode
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
}
