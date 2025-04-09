#include "WindArgParser.h"
#include "lib/RP/RPGolConfig.h"
#include "lib/RP/RPGolDefine.h"
#include <string>
#include <cassert>
#include <iostream>

/// <summary>
/// Parse a command-line argument into a wind set.
/// Expects 8 winds delimited by commas (no spaces)
/// </summary>
/// <param name="arg">Command-line argument</param>
/// <param name="out">Output wind set</param>
bool WindArgParser::parseTargetWindSet(const std::string &arg, RPGolWindSet &out)
{
    // Split the string by its commas
    u64 pos = 0;
    u64 commaPos = 0;
    u32 numWinds = 0;

    std::string wind;
    while (numWinds < RPGolDefine::HOLE_SIZE && pos < arg.length())
    {
        // Get substring of i'th wind
        commaPos = arg.find(",", pos);
        if (commaPos == std::string::npos)
        {
            commaPos = arg.length();
        }

        wind = arg.substr(pos, commaPos - pos);

        pos = commaPos + 1;

        // std::cout << "-----------------------------------------" << std::endl;
        // std::cout << "pos: " << pos << std::endl;
        // std::cout << "commaPos: " << commaPos << std::endl;
        // std::cout << "arg: " << arg << std::endl;
        // std::cout << "arg.length(): " << arg.length() << std::endl;
        // std::cout << "wind: " << wind << std::endl;
        // std::cout << "wind length: " << wind.length() << std::endl;

        std::string speed;
        std::string direction;

        // Read each character of substring and split into speed + direction
        for (u32 j = 0; j < wind.length(); j++)
        {
            // Wildcard ("*")
            if (wind[j] == '*')
            {
                // Speed takes priority
                if (speed.length() == 0)
                {
                    speed.push_back(wind[j]);
                }
                else
                {
                    direction.push_back(wind[j]);
                }
            }
            else if ((wind[j] >= '0') && (wind[j] <= '9'))
            {
                speed.push_back(wind[j]);
            }
            else
            {
                direction.push_back(wind[j]);
            }
        }

        // Convert arg to data

        // std::cout << "speed: " << speed << std::endl;
        // std::cout << "direction: " << direction << std::endl;

        out.mWinds[numWinds].mSpeed = (speed == sWildcardStr) ? RPGolDefine::WILDCARD_SPD : std::stoi(speed);
        out.mWinds[numWinds].mDirection = (direction == sWildcardStr) ? RPGolDefine::WILDCARD_DIR : RPGolDefine::StringToDir(direction.c_str());

        numWinds++;
    }

    while (numWinds < RPGolDefine::HOLE_SIZE)
    {
        out.mWinds[numWinds].mSpeed = RPGolDefine::WILDCARD_SPD;
        out.mWinds[numWinds].mDirection = RPGolDefine::WILDCARD_DIR;
        numWinds++;
    }

    return true;
}
