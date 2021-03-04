#include "WindArgParser.h"
#include <lib/RP/RPGlfConfig.h>
#include <string>
#include <cassert>

/// <summary>
/// Parse a command-line argument into a wind set.
/// Expects 8 winds delimited by commas (no spaces)
/// </summary>
/// <param name="arg">Command-line argument</param>
/// <param name="out">Output wind set</param>
int WindArgParser::parseTargetWindSet(const std::string& arg, RPGlfWindSet& out)
{
	// Expects a full wind set (delimited)
	assert(std::count(arg.begin(), arg.end(), ",") == RPGlfDefine::HOLE_SIZE);

	// Split the string by its commas
	u32 pos;
	std::string wind;
	for (u32 i = 0; i < RPGlfDefine::HOLE_SIZE; i++)
	{
		// Get substring of i'th wind
		pos = arg.find(",");
		wind = arg.substr(pos);

		std::string speed;
		std::string direction;

		// Read each character of substring and split into speed + direction
		for (u32 i = 0; i < wind.length(); i++)
		{
			// Wildcard ("*")
			if (wind[i] == '*')
			{
				// Speed takes priority
				if (speed.length() == 0)
				{
					speed.push_back(wind[i]);
				}
				else
				{
					direction.push_back(wind[i]);
				}
			}
			else if ((wind[i] >= '0') && (wind[i] <= '9'))
			{
				speed.push_back(wind[i]);
			}
			else
			{
				direction.push_back(wind[i]);
			}
		}

		// Convert arg to data
		out.mWinds[i].mSpeed = (speed == "*") ? RPGlfDefine::WindSpd::WILDCARD : std::stoi(speed);
		out.mWinds[i].mDirection = (direction == "*") ? RPGlfDefine::WindDir::WILDCARD : stringToDir(direction);
	}
}
