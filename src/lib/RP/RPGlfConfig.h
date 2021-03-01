#pragma once
#include "types.h"
#include <lib/RP/RPGlfDefine.h>
#include <string>

struct DifficultyInfo;

struct RPGlfConfig
{
	static void makeRandomWind();
	static void makeWindSet(const DifficultyInfo&, const u32*, const u32*);

	static u32 mWindDirections[RPGlfDefine::MAX_WIND_DIV];
	static u32 mWindSpeeds[RPGlfDefine::MAX_WIND_SPD];
};

struct DifficultyInfo
{
	u32 startHole, endHole;
	u32 minWind, maxWind;
};

const u32 CALC_BEFORE_WIND = 9;
const DifficultyInfo diff_Beginner = { 0, 2, 0, 5 };
const DifficultyInfo diff_Intermediate = { 3, 5, 5, 10 };
const DifficultyInfo diff_Expert = { 6, 8, 10, 16 };
const DifficultyInfo diff_Ninehole = { 0, 8, 0, 15 };

const std::string windDirStrings[] = {
	"S", "SE", "E", "NE", "N", "NW", "W", "SW"
};
