#pragma once

#include "lib/RP/RPGolDefine.h"
#include "lib/RP/RPGolWindSet.h"

#include "OgWindFinder.h"

#include "types.h"

#include "server/src/ffi.rs.h"

// struct WindFFI
// {
//     u32 mDirection;
//     s32 mSpeed;
// }

// struct OgWindFinderInputFFIAdd commentMore actions
// {
//     std::vector<Wind> winds;
// };

// struct OgWindFinderOutputFFI
// {
//     u32 seed;
//     std::vector<Wind> winds;
// };

RPGolWindSet inputFFIToWindSet(const OgWindFinderInputFFI &input);
OgWindFinderOutputFFI windSetToOutputFFI(const RPGolWindSet &windSet);
const std::vector<OgWindFinderOutputFFI> &find_og_wind(const OgWindFinderInputFFI &input);