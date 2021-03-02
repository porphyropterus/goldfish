#pragma once
#include "types.h"

typedef s32 reg;

reg rotateLeft(reg input, reg shift) {
    return (input << shift) | ((input >> (32 - shift)) & ~(-1 << shift));
}
#define rlwinm(rA, rS, SH, MASK) \
    rA = rotateLeft(rS, SH) & MASK