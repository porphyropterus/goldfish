#include "OgBlinkPrecomputeGenerator.h"

int main()
{
    OgBlinkPrecomputeGenerator generator(true);
    generator.generateFile("./og_blink_precompute.bin");

    return 0;
}