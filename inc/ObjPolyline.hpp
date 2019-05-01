#ifndef OBJ_POLYLINE_HPP
# define OBJ_POLYLINE_HPP

# include "lib.h"

class DotPolyline;
class TetraTreePolygons;
class DotPolyline;
class Line;

class ObjPolyline
{
public:
	ObjPolyline();
	ObjPolyline(std::vector<DotPolyline *> list_pDot);
	ObjPolyline(ObjPolyline const & ref);
	~ObjPolyline();

	void						free_polyline();
	void						init_formJSON(JSON &json);
	void						analyze_and_update_data();
	void						showDots();

	static std::vector<Dot>		get_list_intersect_dots(
									DotPolyline *pDot1,
									DotPolyline *pDot2);


	std::vector<DotPolyline *>	list_pDot;

	ObjPolyline		&operator=(ObjPolyline const & ref);

private:
	void				_setLocationOf_poliline_dots();
	void				_add_intersectionDots();
	void				_add_timeAndDistance_for_intersectionDots();

	void				_add_list_intersect_dot_to_list_pDot_and_initLocate(
							std::vector<Dot> &list_dot,
							Dot dirention,
							int i);

	static void			_find_intersect(
							TetraTreePolygons *tt_polygons,
							Line line,
							std::vector<Dot> &r_list);

	void				_sort_list_intersect(
								std::vector<Dot> &list_dot,
								Dot dirention);

};

#endif