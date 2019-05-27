#include "lib.h"
#include "LServer.hpp"
// #include "LogisticsController.hpp"
// #include <boost/asio.hpp>

#include "geometry/Dot.hpp"
#include "geometry/Geometry.hpp"
#include "geometry/Line.hpp"

#include <time.h>

void	test_geometry() {
	
	
	Line	line1(Dot(0, 10), Dot(10, 10));
	Line	line2(Dot(5, 5), Dot(15, 5));
	// Dot		dot1(10.5333, 13);

	// std::cerr << Geometry::get_distance(line1) << "\n";

	try {
		Dot dot = Geometry::getDotIntersect(line1, line2);

		std::cerr << dot.x << " : " << dot.y << "\n";
	}
	catch (std::exception & e) {
		std::cerr << "No intersection\n";
	}
}

void	test_time() {
	std::string		s = "2000-05-22T14:56:29.000Z";
	char			z[50];
	struct tm		tm;

	// tm = new struct tm;
	if (strptime(s.c_str(), "%Y-%m-%dT%T.000Z", &tm))
		std::cerr << "hmm\n";
	std::cerr << tm.tm_min << "\n";
	std::cerr << tm.tm_hour << "\n";
	std::cerr << tm.tm_mday << "\n";
	std::cerr << tm.tm_mon << "\n";
	std::cerr << tm.tm_year << "\n";
	std::cerr << tm.tm_wday << "\n";
	std::cerr << tm.tm_yday << "\n";
	std::cerr << tm.tm_isdst << "\n";
}

int main(int argc, char const *argv[])
{
	test_time();
	return 0;
	LServer		server("127.0.0.1", 8080);
	return 0;
}
