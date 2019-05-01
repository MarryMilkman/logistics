#ifndef GEOMETRY_HPP
# define GEOMETRY_HPP

# include "lib.h"
# include "geometry/Line.hpp"

class ObjPolygon;

class Geometry
{
	Geometry() {}
public:
	~Geometry() {}
	
	
	static bool			is_lineHaveDot(Line &line, Dot &dot);
	static Dot			getDotIntersect(Line &line1, Line &line2);
	static bool			is_intersect_line_and_endlessLine(Line &line, Line &endlessLine);
	
	static double			len_from_to_(Dot &dot1, Dot &dot2);
	static bool				is_dotInPolygon(Dot &dot, ObjPolygon *polygon);
	static RelativePosition	whereIs_dotInPolygon(Dot &check_dot, ObjPolygon *polygon);
	static std::vector<Dot>	getDotIntersect(Line &line, ObjPolygon *polugon);

	static Dot				getCenter(Line line);

	static double			get_distance(Line line);

	class	GeometryException : public std::exception
	{
	public:
		GeometryException() throw() {}
		~GeometryException() throw() {}

		virtual const char			*what() const throw() { return ""; }
	};

private:

	static bool			_is_dot_in_square(Dot check_dot, ObjPolygon *polygon);
};

#endif