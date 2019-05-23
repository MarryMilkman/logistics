#ifndef LIB_H
#define LIB_H

#pragma warning(disable: 4996)

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#include <iostream>
#include <sstream>
#include <vector>

#include <fstream>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <libpq-fe.h>
#include <cmath>

#include "geometry/Dot.hpp"

using JSON = nlohmann::json;

std::vector<std::string> split(std::string str);

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

enum	TypeJSON {
	tPolygon,
	tPolyline,
	tError
};

enum	ResultStatus {
	Polygon_added = 0,
	Polygon_intersect,
	Polygon_fullMatch,
	Polygon_errorPartsOfOneArya,

	Polyline_processed,
	Polyline_parseError
};

#endif