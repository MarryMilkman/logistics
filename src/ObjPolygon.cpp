#include "ObjPolygon.hpp"
#include "Plast.hpp"

ObjPolygon::ObjPolygon(t_polygon_data data) {
	this->data = data;
	this->plast = new Plast();
	this->parent = 0;
	this->data.arr_dot = data.arr_dot;
}

ObjPolygon::ObjPolygon(t_polygon_data data, ObjPolygon *parentPolygon) {
	this->data = data;
	this->plast = new Plast();
	this->parent = parentPolygon;
	this->data.arr_dot = data.arr_dot;
}


ObjPolygon::~ObjPolygon() {
	std::cerr << "delete " << this->data.id << "\n";
	if (this->plast)
		delete this->plast;
	this->plast = 0;
	this->parent = 0;
}

ObjPolygon::ObjPolygon(ObjPolygon const & ref) {
	*this = ref;
}

ObjPolygon		&ObjPolygon::operator=(ObjPolygon const & ref) {
	this->plast = ref.plast;
	this->parent = ref.parent;
	this->data = ref.data;
	return *this;
}

void			ObjPolygon::get_log() {
	std::cerr << "--Polygon log:\n";
	std::cerr << "id : " << data.id << "\n";
	std::cerr << "center x: " << data.center.x << "\n";
	std::cerr << "center y: " << data.center.y << "\n";
	std::cerr << "max_x : " << data.max_x << "\n";
	std::cerr << "min_x : " << data.min_x << "\n";
	std::cerr << "max_y : " << data.max_y << "\n";
	std::cerr << "min_y : " << data.min_y << "\n";
	std::cerr << "color : " << data.color << "\n";
	if (this->parent)
		std::cerr << "parent id: " << this->parent->data.id << "\n";
	else
		std::cerr << "parent id: 0" << "\n";
}

// STATIC FUNCTION

bool			ObjPolygon::is_squareOfPolygonsIntersect(ObjPolygon *p1, ObjPolygon *p2) {
	double	max_x_p1 = p1->data.max_x;
	double	max_x_p2 = p2->data.max_x;

	double	min_x_p1 = p1->data.min_x;
	double	min_x_p2 = p2->data.min_x;

	double	max_y_p1 = p1->data.max_y;
	double	max_y_p2 = p2->data.max_y;

	double	min_y_p1 = p1->data.min_y;
	double	min_y_p2 = p2->data.min_y;

	// comparison by X
	if (max_x_p1 < min_x_p2 || min_x_p1 > max_x_p2)
		return false;
	// comparison by Y
	if (max_y_p1 < min_y_p2 || min_y_p1 > max_y_p2)
		return false;
	return true;
}

ObjPolygon		*ObjPolygon::checkDominationSquare(ObjPolygon *p1, ObjPolygon *p2, bool &is_equal) {
	double	max_x_p1 = p1->data.max_x;
	double	max_x_p2 = p2->data.max_x;

	double	min_x_p1 = p1->data.min_x;
	double	min_x_p2 = p2->data.min_x;

	double	max_y_p1 = p1->data.max_y;
	double	max_y_p2 = p2->data.max_y;

	double	min_y_p1 = p1->data.min_y;
	double	min_y_p2 = p2->data.min_y;

	//check for full mach
	if (max_y_p1 == max_y_p2 && min_y_p1 == min_y_p2
	&& max_x_p1 == max_x_p2 && min_x_p1 == min_x_p2) {
		is_equal = true;
		return 0;
	}
	// comparison by X
	if (max_x_p1 < min_x_p2 || min_x_p1 > max_x_p2)
		return 0;
	// comparison by Y
	if (max_y_p1 < min_y_p2 || min_y_p1 > max_y_p2)
		return 0;
	// get dominant square
	if (max_y_p1 >= max_y_p2 && min_y_p1 <= min_y_p2
		&& max_x_p1 >= max_x_p2 && min_x_p1 <= min_x_p2)
		return p1;
	else
		return p2;
}

bool			ObjPolygon::is_polygonsEqual(ObjPolygon *p1, ObjPolygon *p2) {
	std::vector<Dot>	arr_dot1 = p1->data.arr_dot;
	std::vector<Dot>	arr_dot2 = p2->data.arr_dot;
	bool				is_equal;

	for (Dot d1 : arr_dot1) {
		is_equal = false;
		for (Dot d2 : arr_dot2) {
			if (d1.x == d2.x && d1.y == d2.y) {
				is_equal = true;
				break ;
			}
		}
		if (!is_equal)
			return false;
	}
	return true;
}


std::ostream&	operator<<(std::ostream& os, ObjPolygon const  *polygon) {
	os << "Polygon id: ";
	if (polygon)
		os << polygon->data.id << "\n";
	else
		os << "0" << "\n";
	return os;
}


bool			ObjPolygon::is_dot_can_be_in_polygon(Dot dot, ObjPolygon *polygon) {
		if (!polygon)
		return false;

	if (dot.x > polygon->data.max_x || dot.x < polygon->data.min_x || dot.y > polygon->data.max_y || dot.y < polygon->data.min_y)
		return false;
	return true;
}
