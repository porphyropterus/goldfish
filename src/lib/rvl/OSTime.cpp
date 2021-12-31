#include "OSTime.h"

void OSCalendarTimeToDolphinRTC(char* buf, const OSCalendarTime& time)
{
    // Date format string (Same format as dolphin RTC window)
    constexpr const char rtcExample[] = "01/01/0001 00:00:00 AM";
    constexpr const char* rtcFormat = "%2d/%2d/%2d %2d:%2d:%2d %s";

    // Hour is converted from military time
    bool isPm = false;
    u32 hour = time.hour;
    if (hour > 12)
    {
        hour -= 12;
        isPm = true;
    }
    
    std::snprintf(buf, sizeof(rtcExample), rtcFormat, time.mon, time.mday, time.year,
        hour, time.min, time.sec, isPm ? "PM" : "AM");
}
