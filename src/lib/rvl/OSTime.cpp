#include "OSTime.h"
#include <string>

std::string OSCalendarTimeToDolphinRTC(const OSCalendarTime& time)
{
	// Hour is converted from military time
	bool isPm = false;
	u32 timeHour;
	if (time.hour > 12)
	{
		timeHour = time.hour - 12;
		isPm = true;
	}

	// Hour/minute/second must be two digits wide
	char buf[2];
	
	std::sprintf(buf, "%2d", timeHour);
	std::string hour = buf;
	
	std::sprintf(buf, "%2d", time.min);
	std::string min = buf;

	std::sprintf(buf, "%2d", time.sec);
	std::string sec = buf;

	return std::to_string(time.mon) + "/" + std::to_string(time.mday) + "/" + std::to_string(time.year) +
		" " + hour + ":" + min + ":" + sec;
}
