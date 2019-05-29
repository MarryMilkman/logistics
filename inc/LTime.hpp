#ifndef L_TIME_HPP
# define L_TIME_HPP

#include "lib.h"

#define	TIME_FORMAT	"%d-%d-%dT%d:%d:%d.000Z"

class LTime
{
	LTime() {}
public:
	~LTime() {}

	static time_t	get_unix_second(std::string str, const char *format);
};

#endif