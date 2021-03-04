#pragma once
#include "types.h"
#include <lib/RP/RPGlfWindSet.h>
#include <string>

struct WindArgParser
{
	static int parseTargetWindSet(const std::string&, RPGlfWindSet&);
};
