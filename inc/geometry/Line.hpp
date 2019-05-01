#ifndef LINE_HPP
# define LINE_HPP

#include "geometry/Dot.hpp"

class Line
{
public:
	Line(Dot dot1, Dot dot2);
	Line(Line const & ref);
	~Line();
	
	Dot		dot1;
	Dot		dot2;

	Line		&operator=(Line const &ref);
};

#endif