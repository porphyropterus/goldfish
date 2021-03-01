#pragma once
#include "types.h"
#include "lib/RP/RPGlfDefine.h"

enum RPGolGamemode
{
	/* 0x00 */ GOL_COURSE_SELECT,

	/* 0x01 */ GOL_TR_ONEPAT,
	/* 0x02 */ GOL_TR_NEARPIN,
	/* 0x03 */ GOL_TR_TARGET,

	/* 0x04 */ GOL_FT_ONEPAT,
	/* 0x05 */ GOL_FT_NEARPIN,
	/* 0x06 */ GOL_FT_TARGET,

	/* 0x07 */ GOL_NINEHOLE,

	/* 0x08 */ GOL_3H_BEGINNER,
	/* 0x09 */ GOL_3H_INTERMED,
	/* 0x0A */ GOL_3H_EXPERT,
};

struct DifficultyInfo
{
	u32 startHole, endHole;
	u32 minWind, maxWind;
};

const DifficultyInfo diff_Beginner = { 0, 2, 0, 5 };
const DifficultyInfo diff_Intermediate = { 3, 5, 5, 10 };
const DifficultyInfo diff_Expert = { 6, 8, 10, 16 };
const DifficultyInfo diff_Ninehole = { 0, 8, 0, 15 };

const u32 CALC_BEFORE_WIND = 8;

void makeWindSet(const DifficultyInfo&);
void makeRandArrayU32(u32, u32*);
