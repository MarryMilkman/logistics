#ifndef LIB_H
#define LIB_H

#include <iostream>
#include <vector>

#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <libpq-fe.h>
#include <cmath>

#include "geometry/Dot.hpp"

using JSON = nlohmann::json;

enum	IntersectionType {
	PolygonInclude = 0,
	PolygonUpper,
	Intersection_true,
	Intersection_false,
	FullMatch,
	ErrorPartsOfOneArya
};

enum	RelativePosition {
	Outside = 0,
	Inside,
	Border
};

#endif