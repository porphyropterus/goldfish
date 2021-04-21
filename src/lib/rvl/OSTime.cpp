#include "OSTime.h"
#include <string>

std::string OSCalendarTimeToDolphinRTC(const OSCalendarTime& time)
{
    // Hour is converted from military time
    bool isPm = false;
    u32 timeHour = time.hour;
    if (timeHour > 12)
    {
        timeHour -= 12;
        isPm = true;
    }

    // Hour/minute/second must be two digits wide
    char buf[2];
    
    sprintf_s(buf, "%2d", timeHour);
    std::string hour = buf;
    
    sprintf_s(buf, "%2d", time.min);
    std::string min = buf;

    sprintf_s(buf, "%2d", time.sec);
    std::string sec = buf;

    return std::to_string(time.mon) + "/" + std::to_string(time.mday) + "/" + std::to_string(time.year) +
        " " + hour + ":" + min + ":" + sec;
}
