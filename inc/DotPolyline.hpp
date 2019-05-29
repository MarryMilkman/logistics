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
	time_t						sumTime;
	bool						isIntersect;
	
	RelativePosition			position;

	std::vector<ObjPolygon *>	listOf_contactAreas;
	ObjPolygon					*currentArea;
	ObjPolygon					*previousArea;
	ObjPolygon					*nextArea;

	double						distance;

	void						initLocation(ObjPolygon *mostPossiblePolygon);

	void						init_current_time();
	
	DotPolyline					&operator=(DotPolyline const &ref);

private:
	ObjPolygon					*_get_start_polygon(ObjPolygon *mostPossiblePolygon);
	void						_initLocation_basePolygon(ObjPolygon *basePolygon);
	RelativePosition			_initLocation_tt_polygons(
									TetraTreePolygons *tt_polygons,
									std::vector<ObjPolygon *>	&nestedPolygons);




};

#endif