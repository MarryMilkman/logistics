#ifndef PLAST_HPP
# define PLAST_HPP

# include "lib.h"

class ObjPolygon;

class TetraTreePolygons;

class Plast
{
public:
	Plast();
	Plast(TetraTreePolygons *tt_polygons);
	~Plast();
	
	TetraTreePolygons 		*_tt_polygons;
	double					max_x;
	double					max_y;
	double					min_x;
	double					min_y;

	std::vector<ObjPolygon *>	addNewPolygon_toPlast(
									ObjPolygon *polygon,
									ObjPolygon *parentPolygon,
									IntersectionType & answer);
	
	static Plast				*getPlast(ObjPolygon *polygon);

	static Plast				*getOveralPlast(ObjPolygon *p1, ObjPolygon *p2);

	static bool					is_dot_can_be_in_plast(Dot dot, Plast *plast);

	bool						empty();

private:

	void						_update_max_min_coord(ObjPolygon *polygon);

};

#endif