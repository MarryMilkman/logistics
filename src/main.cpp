#include "lib.h"
#include "LServer.hpp"
// #include "LogisticsController.hpp"

#include "geometry/Dot.hpp"
#include "geometry/Geometry.hpp"
#include "geometry/Line.hpp"

void	getresult(JSON &json) {
	std::cout << "RESULT:\n" << json << "\n";
}

void	test_geometry() {
	
	
	Line	line1(Dot(1, 1), Dot(0, 0));
	// Line	line1(Dot(30, 3), Dot(30, 125));
	// Dot		dot1(10.5333, 13);

	std::cerr << Geometry::get_distance(line1) << "\n";

	// try {
	// 	Dot dot = Geometry::getDotIntersect(line1, line2);

	// 	std::cerr << dot.x << " : " << dot.y << "\n";
	// }
	// catch (std::exception & e) {
	// 	std::cerr << "No intersection\n";
	// }
}

int main(int argc, char const *argv[])
{
	LServer		server("localhost", 8080);

	return 0;
}

// int		main(int argc, char const **argv)
// {

// 	// test_geometry();
// 	// exit(0);

// 	LogisticsController	&controller = LogisticsController::getInstance();

// 	controller.reloadPolygonsFrom_db();
// 	// exit(0);
// 	// controller.showSituationWithPolygons();

// 	// work whith HTTP
// 	// but now I wona work with standart input || file =)
// 	JSON			json;
// 	std::string		line;
// 	std::ifstream	file;
// 	bool			isP_line = true;

// 	while (true) {
// 		getline(std::cin, line);
// 		if (line == "exit")
// 			break ;
// 		if (line[0] == 'g')
// 			isP_line = false;
// 		else
// 			isP_line = true;
// 		line = "../resources/" + line;
// 		file.open(line);
// 		if (!file.is_open()) {
// 			std::cerr << "Can't find file " << line << "\n";
// 			continue ;
// 		}
// 		file >> json;
// 		if (isP_line) {
// 			int					i = -1;
// 			int					size = json.size();
// 			// ResultController	result;

// 			while (++i < size) {
// 				controller.init_polyline(json[i]);
// 				// result.addResult(controller.polyline, controller.status);
// 			}
// 			// std::cout << result.get_json() << "\n";
// 		}
// 		else {
// 			int					i = -1;
// 			int					size = json.size();
// 			// ResultController	result;
			
// 			std::cerr << "Start addPolygon_to_controller_and_db\n";
// 			while (++i < size) {
// 				controller.addPolygon_to_controller_and_db(json[i]);
// 				// result.addResult(controller.status)
// 			}
// 			// std::cout << result.get_json() << "\n";
// 		}
// 		std::cerr << controller.get_result().dump(2) << "\n\n";
// 		file.close();
// 	}

// 	// if (JsonController::check)
// 	//
// 	return 0;
// }

