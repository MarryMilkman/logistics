#include "LTime.hpp"

time_t		LTime::get_unix_second(std::string str, const char *format) {
	struct tm		tm;
	int				i;
	time_t 			ts = 0;

	memset(&tm, 0, sizeof(struct tm));
	i = sscanf(str.c_str(), format,
				&tm.tm_year,
				&tm.tm_mon,
				&tm.tm_mday,
				&tm.tm_hour,
				&tm.tm_min,
				&tm.tm_sec);

	std::cerr << tm.tm_year << "\n";
	std::cerr << tm.tm_mon << "\n";
	std::cerr << tm.tm_mday << "\n";
	std::cerr << tm.tm_hour << "\n";
	std::cerr << tm.tm_min << "\n";
	std::cerr << tm.tm_sec << "\n";

	tm.tm_hour -= 1;
	tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    tm.tm_isdst = 0;

	ts = mktime(&tm);

	std::cerr << (int)ts << "\n\n";
	return ts;
}