#pragma once
#include "types.h"

typedef s32 sreg;
typedef u32 ureg;

#ifdef __ROTATE
// https://stackoverflow.com/questions/10134805/bitwise-rotate-left-function
ureg rotl32(ureg input, ureg shift) {
    return (input << shift) | ((input >> (32 - shift)) & ~(-1 << shift));
}
#else
ureg rotl32(const ureg value, ureg shift) {
    if ((shift &= sizeof(value) * 8 - 1) == 0)
        return value;
    return (value << shift) | (value >> (sizeof(value) * 8 - shift));
}
#endif
#define rlwinm(rA, rS, SH, MASK) \
    rA = rotl32(rS, SH) & MASK
