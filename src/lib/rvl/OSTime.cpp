#include "OSTime.h"
#include <cassert>

#ifdef __MWERKS__
asm u64 OSGetTime()
{
	mftbu r3
	mftbl r4
	mftbl r5
	cmpw r3, r5
	bne OSGetTime
	
	blr
}
#else
u64 OSGetTime()
{
	assert(false);
	return 0;
}
#endif

void OSTicksToCalendarTime(u64 ticks, OSCalendarTime* time)
{

}
