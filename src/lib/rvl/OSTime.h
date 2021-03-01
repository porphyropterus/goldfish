// Modified OSTime.h from BrainSlug
// https://github.com/Chadderz121/brainslug-wii/blob/master/bslug_include/rvl/OSTime.h

/* OSTime.h
 *   by Alex Chadwick
 *
 * Copyright (C) 2014, Alex Chadwick
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once
#include "types.h"

struct OSCalendarTime
{
    /* 0x00 */ s32 sec;
    /* 0x04 */ s32 min;
    /* 0x08 */ s32 hour;
    /* 0x0C */ s32 mday;
    /* 0x10 */ s32 mon;
    /* 0x14 */ s32 year;
    /* 0x18 */ s32 wday;
    /* 0x1C */ s32 yday;
    /* 0x20 */ s32 msec;
    /* 0x24 */ s32 usec;
}; /* sizeof = 0x28 (40u)*/

u64 OSGetTime();
void OSTicksToCalendarTime(u64 ticks, OSCalendarTime* time);

const u32 YearDays[] = {
    0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
};
const u32 LeapYearDays[] = {
    0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335
};

#define TB_BUS_CLOCK				243000000u
#define TB_CORE_CLOCK				729000000u
#define TB_TIMER_CLOCK				(TB_BUS_CLOCK/4000)	

#define ticks_to_secs(ticks)		(((ticks)/(TB_TIMER_CLOCK*1000)))
#define ticks_to_millisecs(ticks)	(((ticks)/(TB_TIMER_CLOCK)))
#define ticks_to_microsecs(ticks)	((((ticks)*8)/(TB_TIMER_CLOCK/125)))
#define ticks_to_nanosecs(ticks)	((((ticks)*8000)/(TB_TIMER_CLOCK/125)))

#define secs_to_ticks(sec)			((sec)*(TB_TIMER_CLOCK*1000))
#define millisecs_to_ticks(msec)	((msec)*(TB_TIMER_CLOCK))
#define microsecs_to_ticks(usec)	(((usec)*(TB_TIMER_CLOCK/125))/8)
#define nanosecs_to_ticks(nsec)		(((nsec)*(TB_TIMER_CLOCK/125))/8000)
