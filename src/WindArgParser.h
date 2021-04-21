#pragma once
#include "types.h"
#include <lib/RP/RPGlfWindSet.h>
#include <string>

namespace WindArgParser
{
    bool parseTargetWindSet(const std::string&, RPGlfWindSet&);
    const std::string sWildcardStr = "*";
};
