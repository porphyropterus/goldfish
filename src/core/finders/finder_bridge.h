#pragma once

#include "lib/RP/RPGolDefine.h"
#include "lib/RP/RPGolWindSet.h"

#include "OgWindFinder.h"

#include "types.h"

struct OgWindFinderInputFFI;

struct OgWindFinderOutputFFI;

RPGolWindSet inputFFIToWindSet(const OgWindFinderInputFFI &input);
OgWindFinderOutputFFI windSetToOutputFFI(const RPGolWindSet &windSet);
std::vector<OgWindFinderOutputFFI> find_og_wind(OgWindFinderInputFFI &input);