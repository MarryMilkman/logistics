#include "geometry/Line.hpp"

Line::Line(Dot dot1, Dot dot2) : dot1(dot1), dot2(dot2) {}

Line::Line(Line const &ref) {
	*this = ref;
}

Line::~Line() {}

Line		&Line::operator=(Line const & ref) {
	this->dot1 = ref.dot1;
	this->dot2 = ref.dot2;
	return *this;
}