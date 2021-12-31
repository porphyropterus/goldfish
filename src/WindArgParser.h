#pragma once
#include "types.h"
#include <lib/RP/RPGolWindSet.h>
#include <string>

namespace WindArgParser
{
    bool parseTargetWindSet(const std::string&, RPGolWindSet&);
    const std::string sWildcardStr = "*";
};
