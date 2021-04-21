#include "RPGlfConfig.h"
#include <lib/RP/RPUtlRandom.h>
#include <lib/RP/RPGlfDefine.h>
#include "DifficultyInfo.h"
#include <cstdio>

/// <summary>
/// Generates a random sequence of numbers 
/// between 0-max (exclusive), with no duplicates.
/// </summary>
/// <param name="max">Upper limit of sequence</param>
/// <param name="pArray">Pointer to array to fill</param>
void RPGlfConfig::makeRandomSequence(s32 max, s32* pArray)
{
    for (s32 i = 0; i < max; i++)
        pArray[i] = -max;

    s32 slotsFilled = 0;
    while (true)
    {
        s32 random = (s32)(RPUtlRandom::getF32() * (max - slotsFilled));

        if (max > 0)
        {
            for (s32 i = 0, j = 0; i < max; i++)
            {
                if ((pArray[j] < 0) && (--random < 0))
                {
                    pArray[j] += max;
                    pArray[slotsFilled++] += j;

                    if (slotsFilled >= max) return;
                    
                    break;
                }
                else j++;
            }
        }
    }
}

/// <summary>
/// Generate a pool of random speeds and directions, and choose 9 of each
/// to represent the "wind set" for the current game.
/// </summary>
/// <param name="diff">Selected difficulty</param>
void RPGlfConfig::chooseWindSet()
{
    RPGlfConfig* instance = getInstance();

    // Simulate all random number generations before wind is generated
    RPUtlRandom::advance(CALC_BEFORE_WIND);

    // Generate random sequences
    // (speed/dir, values this function will hand-pick from)
    s32 randomDirs[randomDirArraySize];
    makeRandomSequence(randomDirArraySize, randomDirs);
    s32 randomSpeeds[randomSpeedArraySize];
    makeRandomSequence(randomSpeedArraySize, randomSpeeds);

    u32 gameLength = mDiff.endHole - mDiff.startHole;

    // Holes that have non-zero wind speed
    u32 numNonzeroWinds = 0;
    u32 r20 = 0;
    u32 r19 = 0;
    u32 r27_i = 0;

    // Loop through each hole
    for (u32 i = 0; i < RPGlfDefine::HOLE_SIZE; i++)
    {
        // Will the [i]'th hole NOT be played
        // with the current difficulty?
        if ((i < mDiff.startHole) || (i > mDiff.endHole))
        {
            // If it won't, use dummy wind (32 S)
            instance->mWindSet.mWinds[i].mDirection = RPGlfDefine::MAX_WIND_DIV;
            instance->mWindSet.mWinds[i].mSpeed = RPGlfDefine::MAX_WIND_SPD;
        }
        else
        {
            s32 nextSpd = 0;
            do
            {
                // Evaluate speeds in random sequence
                nextSpd = randomSpeeds[r20++];
            } while ((nextSpd < mDiff.minWind) && (nextSpd > mDiff.maxWind));

            // It is valid, so let's use it
            instance->mWindSet.mWinds[i].mSpeed = nextSpd;

            if (numNonzeroWinds < 8)
            {
                s32 nextDir = randomDirs[r27_i];
                instance->mWindSet.mWinds[i].mDirection = nextDir;
                if (nextSpd <= 0) continue;
                numNonzeroWinds++;
                r27_i++;
            }
            // If there are eight holes already with non-zero wind,
            // this hole MUST have zero wind.
            else
            {
                // If the next wind speed was going to be zero, we take it
                if (nextSpd == 0)
                {
                    instance->mWindSet.mWinds[i].mDirection = RPGlfDefine::SOUTH;
                    continue;
                }
                // Otherwise, we pick a random hole and override its speed with zero.
                else
                {
                    u32 rndNum = (u32)(RPUtlRandom::getF32() * gameLength) + mDiff.startHole;
                    instance->mWindSet.mWinds[rndNum].mSpeed = 0;
                    // Pick a random direction from existing winds?
                    instance->mWindSet.mWinds[i].mDirection = instance->mWindSet.mWinds[rndNum].mDirection;
                }
            }
        }
    }
}

RPGlfWindSet& RPGlfConfig::getWindSet()
{
    return mWindSet;
}

Wind& RPGlfConfig::getWind(u32 hole)
{
    return mWindSet.mWinds[hole];
}

void RPGlfConfig::setWind(u32 hole, const Wind& newWind)
{
    mWindSet.mWinds[hole] = newWind;
}

DifficultyInfo& RPGlfConfig::getDifficulty()
{
    return mDiff;
}

void RPGlfConfig::setDifficulty(const DifficultyInfo& newDiff)
{
    mDiff = newDiff;
}

RPGlfConfig* RPGlfConfig::getInstance()
{
    INSTANCE_GUARD(RPGlfConfig);
    return mInstance;
}

RPGlfConfig* RPGlfConfig::mInstance = NULL;

/// <summary>
/// Converts a direction string to a WindDir enum value
/// </summary>
/// <param name="s">Direction string</param>
/// <returns>WindDir enum value</returns>
RPGlfDefine::WindDir stringToDir(std::string& s)
{
    u32 len = s.length();
    if (len > 2) return RPGlfDefine::MAX_WIND_DIV;

    // Convert string to uppercase
    for (u32 i = 0; i < s.length(); i++)
        s[i] = std::toupper(s[i]);
    
    switch (len)
    {
    case 1:
        switch (s.at(0))
        {
        case 'N':
            return RPGlfDefine::NORTH;
        case 'S':
            return RPGlfDefine::SOUTH;
        case 'E':
            return RPGlfDefine::EAST;
        case 'W':
            return RPGlfDefine::WEST;
        default:
            return RPGlfDefine::MAX_WIND_DIV;
        }

    case 2:
        if (s.compare("NE") == 0) return RPGlfDefine::NORTHEAST;
        if (s.compare("NW") == 0) return RPGlfDefine::NORTHWEST;
        if (s.compare("SE") == 0) return RPGlfDefine::SOUTHEAST;
        if (s.compare("SW") == 0) return RPGlfDefine::SOUTHWEST;
        return RPGlfDefine::MAX_WIND_DIV;

    default:
        return RPGlfDefine::MAX_WIND_DIV;
    }
}
