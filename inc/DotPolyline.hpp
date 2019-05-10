#ifndef DOT_POLYLINE_HPP
# define DOT_POLYLINE_HPP

# include "lib.h"

class ObjPolygon;
class Plast;
class TetraTreePolygons;

class DotPolyline
{
public:
	DotPolyline();
	DotPolyline(Dot dot, std::string current_time);
	DotPolyline(DotPolyline const &ref);
	~DotPolyline();

	int							id;
	Dot							dot;
	std::string					current_time;
	bool						isIntersect;
	RelativePosition			position;
	std::vector<ObjPolygon *>	listOf_contactAreas;
	ObjPolygon					*currentArea;
	ObjPolygon					*previousArea;
	ObjPolygon					*nextArea;

	double						distance;

	void						initLocation(ObjPolygon *mostPossiblePolygon);
	RelativePosition			initLocation_plast(Plast *plast);

	DotPolyline					&operator=(DotPolyline const &ref);

private:
	RelativePosition	_initLocation_tt_polygons(TetraTreePolygons *tt_polygons);




};

#endif