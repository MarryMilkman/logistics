#include "lib.h"
#include "LServer.hpp"
// #include "LogisticsController.hpp"

#include "geometry/Dot.hpp"
#include "geometry/Geometry.hpp"
#include "geometry/Line.hpp"

#include <ctime>

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


int main(int argc, char const *argv[])
{
	LServer		server("127.0.0.1", 8080);
	return 0;
}
