#include "geometry/Geometry.hpp"
#include "ObjPolygon.hpp"

// a1x + b1y + c1 = 0
// a2x + b2y + c2 = 0
// x = (c2 * b1 - c1 * b2) / (a1 * b2 - a2 * b1)
// y = (c2 * a1 - c1 * a2) / (a2 * b1 - a1 * b2)

std::vector<Dot>	Geometry::getDotIntersect(Line &line, ObjPolygon *polygon) {
	if (!polygon || (!Geometry::is_dot_in_square(line.dot1, polygon)
				&& !Geometry::is_dot_in_square(line.dot2, polygon)) )
		std::vector<Dot>();

	std::vector<Dot>	arr_dot = polygon->data.arr_dot;
	std::vector<Dot>	intersect_arr;
	Dot					dot1;
	Dot					dot2;
	int					i = -1;
	int					size = arr_dot.size();

	while (++i < size - 1) {
		dot1 = arr_dot[i];
		dot2 = arr_dot[i + 1];
		Line	polygon_line(dot1, dot2);

		try {
			Dot	dotInterset = Geometry::getDotIntersect(line, polygon_line);

			if (dotInterset == dot1)
				continue;
			intersect_arr.insert(intersect_arr.end(), dotInterset);
		}
		catch (std::exception & e) {
		}
	}
	return intersect_arr;
}

Dot					Geometry::getDotIntersect(Line &line1, Line &line2) {
	if (line1.dot1 == line2.dot1 || line1.dot1 == line2.dot2)
		return line1.dot1;
	if (line2.dot1 == line1.dot1 || line2.dot1 == line1.dot2)
		return line2.dot1;
	// FIRST PART
	/////////////////
		// line1:  
			//max and min x
	double max_x_line1;
	double min_x_line1;

	if  (line1.dot1.x > line1.dot2.x) {
		max_x_line1 = line1.dot1.x;
		min_x_line1 = line1.dot2.x;
	}
	else {
		max_x_line1 = line1.dot2.x;
		min_x_line1 = line1.dot1.x;
	}
			//max and min y
	double max_y_line1;
	double min_y_line1;

	if  (line1.dot1.y > line1.dot2.y) {
		max_y_line1 = line1.dot1.y;
		min_y_line1 = line1.dot2.y;
	}
	else {
		max_y_line1 = line1.dot2.y;
		min_y_line1 = line1.dot1.y;
	}
		// line2:  
			//max and min x
	double max_x_line2;
	double min_x_line2;

	if  (line2.dot1.x > line2.dot2.x) {
		max_x_line2 = line2.dot1.x;
		min_x_line2 = line2.dot2.x;
	}
	else {
		max_x_line2 = line2.dot2.x;
		min_x_line2 = line2.dot1.x;
	}
			//max and min y
	double max_y_line2;
	double min_y_line2;

	if  (line2.dot1.y > line2.dot2.y) {
		max_y_line2 = line2.dot1.y;
		min_y_line2 = line2.dot2.y;
	}
	else {
		max_y_line2 = line2.dot2.y;
		min_y_line2 = line2.dot1.y;
	}

	if (max_x_line1 < min_x_line2 || min_x_line1 > max_x_line2
		|| max_y_line1 < min_y_line2 || min_y_line1 > max_y_line2)
		throw Geometry::GeometryException();
		

	// SECOND PART
	/////////////////
	double x;
	double y;

	double a1 = line1.dot1.y - line1.dot2.y;
	double a2 = line2.dot1.y - line2.dot2.y;

	double b1 = line1.dot2.x - line1.dot1.x;
	double b2 = line2.dot2.x - line2.dot1.x;

	double c1 = line1.dot1.x * line1.dot2.y - line1.dot1.y * line1.dot2.x;
	double c2 = line2.dot1.x * line2.dot2.y - line2.dot1.y * line2.dot2.x;

	if (!(a1 * b2 - a2 * b1) || !(a2 * b1 - a1 * b2))
		throw Geometry::GeometryException();

	x = (c2 * b1 - c1 * b2) / (a1 * b2 - a2 * b1);
	if (x > max_x_line1 || x > max_x_line2 || x < min_x_line1 || x < min_x_line2)
		throw Geometry::GeometryException();
	y = (c2 * a1 - c1 * a2) / (a2 * b1 - a1 * b2);
	if (y > max_y_line1 || y > max_y_line2 || y < min_y_line1 || y < min_y_line2)
		throw Geometry::GeometryException();

	return Dot(x, y);
}

 // a * x + b * y + c = 0
bool			Geometry::is_lineHaveDot(Line &line, Dot &dot) {
	double x = dot.x;
	double y = dot.y;
	double a = line.dot1.y - line.dot2.y;
	double b = line.dot2.x - line.dot1.x;
	double c = line.dot1.x * line.dot2.y - line.dot1.y * line.dot2.x;

	if ((a * x + b * y + c) <= 0.00001 && (a * x + b * y + c) >= -0.00001)
		return true;
	return false;
}

bool			Geometry::is_intersect_line_and_endlessLine(Line &line, Line &endlessLine) {
	double max_x_line;
	double min_x_line;

	if  (line.dot1.x > line.dot2.x) {
		max_x_line = line.dot1.x;
		min_x_line = line.dot2.x;
	}
	else {
		max_x_line = line.dot2.x;
		min_x_line = line.dot1.x;
	}
			//max and min y
	double max_y_line;
	double min_y_line;

	if  (line.dot1.y > line.dot2.y) {
		max_y_line = line.dot1.y;
		min_y_line = line.dot2.y;
	}
	else {
		max_y_line = line.dot2.y;
		min_y_line = line.dot1.y;
	}

	double x;
	double y;

	double a1 = line.dot1.y - line.dot2.y;
	double a2 = endlessLine.dot1.y - endlessLine.dot2.y;

	double b1 = line.dot2.x - line.dot1.x;
	double b2 = endlessLine.dot2.x - endlessLine.dot1.x;

	double c1 = line.dot1.x * line.dot2.y - line.dot1.y * line.dot2.x;
	double c2 = endlessLine.dot1.x * endlessLine.dot2.y - endlessLine.dot1.y * endlessLine.dot2.x;

	if (!(a1 * b2 - a2 * b1) || !(a2 * b1 - a1 * b2))
		throw Geometry::GeometryException();
	
	x = (c2 * b1 - c1 * b2) / (a1 * b2 - a2 * b1);
	if (x > max_x_line || x < min_x_line)
		return false;
	y = (c2 * a1 - c1 * a2) / (a2 * b1 - a1 * b2);
	if (y > max_y_line || y < min_y_line)
		return false;
	return true;
}

// Len from dot to dot (line len)

double		len_from_to_(Dot &dot1, Dot &dot2) {
		//max and min x
	double max_x;
	double min_x;

	if  (dot1.x > dot2.x) {
		max_x = dot1.x;
		min_x = dot2.x;
	}
	else {
		max_x = dot2.x;
		min_x = dot1.x;
	}
		//max and min y
	double max_y;
	double min_y;

	if  (dot1.y > dot2.y) {
		max_y = dot1.y;
		min_y = dot2.y;
	}
	else {
		max_y = dot2.y;
		min_y = dot1.y;
	}

	// calcuate len
	double lenX = max_x - min_x;
	double lenY = max_y - min_y;

	return sqrt(lenX * lenX + lenY * lenY);
}

// MARK: is dot in polygon true || false 
// let the vertical line and check
// whether it intersects with the line of polygon
bool			Geometry::is_dotInPolygon(Dot &dot, ObjPolygon *polygon) {
	RelativePosition	answer = Geometry::whereIs_dotInPolygon(dot, polygon);

	if (answer == RelativePosition::Outside)
		return false;
	return true;
}

// MARK: - where is dot in polygon?
// return RelativePosition outside\inside\border

RelativePosition		Geometry::whereIs_dotInPolygon(Dot &check_dot, ObjPolygon *polygon) {
	if (!polygon || !Geometry::is_dot_in_square(check_dot, polygon))
		return RelativePosition::Outside;

	Line				check_line(check_dot, Dot(check_dot.x, polygon->data.max_y));
	std::vector<Dot>	arr_dot = polygon->data.arr_dot;
	Dot					dot1;
	Dot					dot2;
	int					i = -1;
	int					size = arr_dot.size();
	int					countIntersect = 0;

	while (++i < size - 1) {
		dot1 = arr_dot[i];
		dot2 = arr_dot[i + 1];
		Line	polygon_line(dot1, dot2);

		if (Geometry::is_lineHaveDot(polygon_line, check_dot))
			return RelativePosition::Border;

		try {
			Dot	answer_dot = Geometry::getDotIntersect(check_line, polygon_line);

			if (answer_dot.x == check_dot.x && answer_dot.y == check_dot.y)
				return RelativePosition::Border;
			if (answer_dot.x == dot1.x && answer_dot.y == dot1.y)
				continue;
			if (answer_dot.x == dot2.x && answer_dot.y == dot2.y) {
				Dot		additionalDot1 = dot1;
				Dot		additionalDot2 = i + 2 < size ? arr_dot[i + 2] : arr_dot[0];
				Line	additionalLine(additionalDot1, additionalDot2);

				if (Geometry::is_intersect_line_and_endlessLine(additionalLine, check_line) == false)
					continue;
			}
			countIntersect++;
		}
		catch (std::exception & e) {
		}
	}
	if ((countIntersect + 2) % 2)
		return RelativePosition::Inside;
	return RelativePosition::Outside;
}

Dot		Geometry::getCenter(Line line) {
	double		x = (line.dot1.x + line.dot2.x) / 2;
	double		y = (line.dot1.y + line.dot2.y) / 2;

	return Dot(x, y);
}

bool	Geometry::is_dot_in_square(Dot check_dot, ObjPolygon *polygon) {
	if (!polygon)
		return true;
	double	x = check_dot.x;
	double	y = check_dot.y;

	double	max_x = polygon->data.max_x;
	double	min_x = polygon->data.min_x;

	double	max_y = polygon->data.max_y;
	double	min_y = polygon->data.min_y;

	if (x > max_x || x < min_x || y > max_y || y < min_y)
		return false;
	return true;
}

// arccos {(sin(широта1) * sin (широта2) + cos (широта1) * cos (широта2) * cos (долгота1 - долгота2)}

double			Geometry::get_distance(Line line) {
	double	x1 = line.dot1.x * 3.1415 / 180;
	double	x2 = line.dot2.x * 3.1415 / 180;

	double	y1 = line.dot1.y * 3.1415 / 180;
	double	y2 = line.dot2.y * 3.1415 / 180;

	double	result;

	result = acos( sin(x1) * sin(x2) + cos(x1) * cos(x2) * cos(y1 - y2) ) * 6356.752;
	return result;
}
