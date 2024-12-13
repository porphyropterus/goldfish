#include <cstdlib>
#include <cmath>
#include "RPGolWindSet.h"
#include "RPGolDefine.h"
#include "RPGolConfig.h"

unsigned int RPGolWindSet::hashWithDepth(int depth)
{
    unsigned int hash = 0;

    for (int i = 0; i < depth; i++)
    {
        unsigned int combined = (mWinds[i].mSpeed & 0xF) << 3 | (mWinds[i].mDirection & 0x7);
        hash = (hash << 7) | combined;
    }

    return hash;
}

/// <summary>
/// Score a wind set against a target, with the score representing how close
/// it is to its target set. Used to rank seeds in order of how close they are
/// to what wind the user wants.
/// </summary>
/// <param name="target">Target wind set to score against</param>
/// <returns></returns>
Score_t RPGolWindSet::scoreAgainst(const RPGolWindSet &target) const
{
    Score_t myScore = 0;
    for (u32 i = 0; i < RPGolDefine::HOLE_SIZE; i++)
    {
        // Wildcard counts as them matching
        if (mWinds[i].mDirection == RPGolDefine::WILDCARD_DIR)
        {
            myScore += scoreBase;
        }
        // Base score - difference in wind direction
        else
        {
            myScore += scoreBase - std::abs((int)mWinds[i].mDirection - (int)target.mWinds[i].mDirection);
        }
        // The speed score is incremented by 1 and then halved
        // to have about the same priority in the total score as the direction would.
        // (Same wildcard rules apply)
        if (mWinds[i].mSpeed == RPGolDefine::WILDCARD_SPD)
        {
            myScore += scoreBase;
        }
        else
        {
            myScore += scoreBase - (std::abs((int)mWinds[i].mSpeed - (int)target.mWinds[i].mSpeed) + 1) / 2;
        }
    }

    return myScore;
}

/// <summary>
/// Returns data in wind set in string format
/// Custom delimiter support for things like CSV data analysis
/// </summary>
/// <returns>Wind set string</returns>
void RPGolWindSet::toString(char *out, const char *setStartDelim, const char *setEndDelim,
                            const char *termStartDelim, const char *termEndDelim, bool bCloseEndDelim) const
{
    // String buffer
    char buf[1024];

    // Insert set start delimiter
    std::strcat(buf, setStartDelim);

    // Convert each speed + direction to string (keep wildcard if that was part of the input)
    for (u32 i = 0; i < RPGolDefine::HOLE_SIZE; i++)
    {
        // Term start delim
        std::strcat(buf, termStartDelim);

        // Wind speed/direction
        char windbuf[128];
        std::snprintf(windbuf, sizeof(windbuf), "%2dm/s %s", mWinds[i].mSpeed, RPGolDefine::DirToString(mWinds[i].mDirection));
        std::strcat(buf, windbuf);

        // Term end delim (except last item)
        if (i < RPGolDefine::HOLE_SIZE - 1 || bCloseEndDelim)
            std::strcat(buf, termEndDelim);
    }

    // Insert set end delimiter
    std::strcat(buf, setEndDelim);

    // Copy out full string
    std::strcpy(out, buf);
}