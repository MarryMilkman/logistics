#ifndef DOT_HPP
#define DOT_HPP

class Dot
{
public:
	Dot();
	Dot(double x, double y);
	Dot(Dot const &ref);
	~Dot();
	
	double	x;
	double	y;

	Dot		&operator=(Dot const &ref);
	bool	operator==(Dot const & ref);
};

#endif