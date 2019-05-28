
#include "lib.h"
// #include "LServer.hpp"
#include "LogisticsController.hpp"

// #include <iostream>
// #include <nlohmann/json.hpp>

// int main(int argc, char const *argv[])
// {
// 	std::cout << "Hello world\n";
// 	return 0;
// }


int		main(int argc, char const **argv)
{

	// test_geometry();
	// exit(0);

	LogisticsController	&controller = LogisticsController::getInstance();

	controller.reloadPolygonsFrom_db();
	// exit(0);
	// controller.showSituationWithPolygons();

	// work whith HTTP
	// but now I wona work with standart input || file =)
	JSON			json;
	std::string		line;
	std::ifstream	file;
	bool			isP_line = true;

	while (true) {
		getline(std::cin, line);
		if (line == "exit")
			break ;
		if (line[0] == 'g')
			isP_line = false;
		else
			isP_line = true;
		line = "../resources/" + line;
		file.open(line);
		if (!file.is_open()) {
			std::cerr << "Can't find file " << line << "\n";
			continue ;
		}
		file >> json;
		if (isP_line) {
			int					i = -1;
			int					size = json.size();
			// ResultController	result;

			while (++i < size) {
				controller.init_polyline(json[i]);
				// result.addResult(controller.polyline, controller.status);
			}
			// std::cout << result.get_json() << "\n";
		}
		else {
			int					i = -1;
			int					size = json.size();
			// ResultController	result;
			
			std::cerr << "Start addPolygon_to_controller_and_db\n";
			while (++i < size) {
				controller.addPolygon_to_controller_and_db(json[i]);
				// result.addResult(controller.status)
			}
			// std::cout << result.get_json() << "\n";
		}
		std::cerr << controller.get_result().dump(2) << "\n\n";
		file.close();
	}

	// if (JsonController::check)
	//
	return 0;
}

