#include "geometry/Dot.hpp"

Dot::Dot() : x(0), y(0) {
}

Dot::Dot(double x, double y) : x(x), y(y) {}

Dot::~Dot() {}

Dot::Dot(Dot const &ref) {
	*this = ref;
}

Dot		&Dot::operator=(Dot const & ref) {
	this->x = ref.x;
	this->y = ref.y;
	return *this;
}


bool	Dot::operator==(Dot const & ref) {
	if (this->x == ref.x && this->y == ref.y)
		return true;
	return false;
}