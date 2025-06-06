#include "WsrWindPrecomputeGenerator.h"

int main()
{
    WsrWindPrecomputeGenerator generator;
    generator.generateFile("./wsr_wind_precompute.bin");

    return 0;
}