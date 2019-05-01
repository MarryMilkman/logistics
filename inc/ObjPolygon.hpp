#ifndef OBJ_POLYGON_HPP
#define OBJ_POLYGON_HPP

#include "lib.h"
#include "t_polygon_data.h"

class Plast;

class ObjPolygon
{
public:
	ObjPolygon(t_polygon_data data);
	ObjPolygon(t_polygon_data data, ObjPolygon *parentPolygon);
	ObjPolygon(ObjPolygon const &ref);
	~ObjPolygon();


	t_polygon_data		data;
	ObjPolygon			*parent;
	Plast				*plast;

	std::vector<ObjPolygon *>	otherParts;
	
	void				get_log();

	static bool			is_polygonsEqual(ObjPolygon *p1, ObjPolygon *p2);
	static bool			is_squareOfPolygonsIntersect(ObjPolygon *p1, ObjPolygon *p2);
	static ObjPolygon	*checkDominationSquare(ObjPolygon *p1, ObjPolygon *p2, bool &is_equal);

	ObjPolygon			&operator=(ObjPolygon const &ref);
private:


};

#endif