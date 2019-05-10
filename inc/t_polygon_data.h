#ifndef T_POLYGON_DATA_H
#define T_POLYGON_DATA_H

#include "vector"
#include "geometry/Dot.hpp"

typedef struct				t_polygon_data {
	std::string			id;

	std::vector<Dot>	arr_dot;

	double				max_x;
	double				max_y;
	double				min_x;
	double				min_y;

	std::string			color;
	
	Dot					center;
}							t_polygon_data;

#endif