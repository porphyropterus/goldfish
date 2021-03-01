#include <iostream>
#include "types.h"
#include "lib/RP/RPUtlRandom.h"
#include "lib/rvl/OSTime.h"
#include "GolfWind.h"

int main()
{
	OSCalendarTime ctime;
	ctime.sec = 0;
	ctime.min = 1;
	ctime.hour = 6;
	ctime.mday = 3;
	ctime.mon = 4;
	ctime.year = 2020;
	ctime.wday = 0;
	ctime.yday = 123;
	ctime.msec = 595;
	ctime.usec = 111;

	RPUtlRandom::setSeed(ctime.min << 26 | ctime.sec << 20 | ctime.msec << 10 | ctime.usec);

	makeWindSet(diff_Ninehole);

	return 0;
}
