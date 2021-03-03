#pragma once
#include "types.h"
#include <lib/RP/RPGlfDefine.h>
#include <string>

struct DifficultyInfo;

class RPGlfConfig
{
public:
	static void makeRandomSequence(s32, s32*);        // at 8029dea8
	static void chooseWindSet(const DifficultyInfo&); // at 8029dcf4

	static u32* getWindDirs();
	static s32* getWindSpeeds();

private:
	inline RPGlfConfig() : mWindDirs(), mWindSpeeds() {}
	u32 mWindDirs[RPGlfDefine::HOLE_SIZE];
	s32 mWindSpeeds[RPGlfDefine::HOLE_SIZE];

	static RPGlfConfig* mInstance;
	static RPGlfConfig* getInstance();
};

struct DifficultyInfo
{
	u32 startHole, endHole;
	u32 minWind, maxWind;
};

// Array sizes for makeRandomSequence
const u32 randomSpeedArraySize = 16;
const u32 randomDirArraySize = 8;

// Number of RNG calls before makeWindSet.
// Ex: seed must be updated 9 times between its instantiation
// and makeWindSet being called to be accurate
const u32 CALC_BEFORE_WIND = 9;

const DifficultyInfo diff_Beginner = { 0, 2, 0, 5 };
const DifficultyInfo diff_Intermediate = { 3, 5, 5, 10 };
const DifficultyInfo diff_Expert = { 6, 8, 10, 16 };
const DifficultyInfo diff_Ninehole = { 0, 8, 0, 15 };

const std::string windDirStrings[] = {
	"S", "SE", "E", "NE", "N", "NW", "W", "SW"
};
