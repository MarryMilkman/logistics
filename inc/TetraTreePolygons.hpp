#ifndef TERTA_TREE_POLYGONS_HPP
# define TERTA_TREE_POLYGONS_HPP

# include "lib.h"



class ObjPolygon;

class TetraTreePolygons
{
public:
	TetraTreePolygons();
	TetraTreePolygons(ObjPolygon *polygon);
	~TetraTreePolygons();

	TetraTreePolygons	*moreX_lessY;
	TetraTreePolygons	*moreX_moreY;
	TetraTreePolygons	*lessX_moreY;
	TetraTreePolygons	*lessX_lessY;

	ObjPolygon				*polygon;

	void					addNewPolygon(ObjPolygon *polygon);
	void					deleteBranches();

	//static functions

	static TetraTreePolygons				*getUpdate_tt_polygons_without(
												TetraTreePolygons *old_tt_polygons,
												std::vector<ObjPolygon *> polygons);

	static bool								is_existence(
												ObjPolygon *check_polygon,
												std::vector<ObjPolygon *> polygons);

	static TetraTreePolygons				*createNew_tt_polygons_with(
												std::vector<ObjPolygon *> polygons);

	static std::vector<TetraTreePolygons *>	createSortListFromChildOf_tt_polygons_withDestination(
												TetraTreePolygons *tt_polygon,
												Dot &destination) ;

	static ObjPolygon						*checkDominationSquare(
												ObjPolygon *p1,
												ObjPolygon *p2,
												bool &is_equal);

	bool									is_squareOfPolygonsIntersect(
												ObjPolygon *p1,
												ObjPolygon *p2);


	static bool								is_polygonsEqual(
												ObjPolygon *p1,
												ObjPolygon *p2);
};

#endif