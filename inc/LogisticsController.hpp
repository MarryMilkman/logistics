#ifndef LOGISTICS_CONTROLLER_HPP
# define LOGISTICS_CONTROLLER_HPP

# include "lib.h"
# include "t_polygon_data.h"

class TetraTreePolygons;
class Plast;
class ObjPolygon;
class ObjPolyline;


class LogisticsController
{
	LogisticsController();
public:
	static LogisticsController	&getInstance();
	LogisticsController(LogisticsController const &ref) = delete;
	LogisticsController &operator=(LogisticsController const &ref) = delete;
	~LogisticsController();

	void				reloadPolygonsFrom_db();
	void				init_polyline(JSON &json);
	void				addPolygon_to_controller_and_db(JSON json_Polygon);
	// JSON				&get_resultJSON();

	static std::vector<ObjPolygon *>	checkIntersection(
											ObjPolygon *check_polygon,
											TetraTreePolygons *check_tt_polygon,
											IntersectionType *answer);

	void								deletePolygonFrom_db(int id);

	void								showSituationWithPolygons();
	Plast								*get_plast();
	// void								_showDotsPolyline();

private:
	ObjPolyline			*_polyline;
	Plast				*_plast;

	// void						_init_polyline_data();
	// void						_init_polyline_data_setLocationOf_poliline_dot();
	// void						_init_polyline_data_addDotOfIntersect();
	// void						_init_polyline_data_addTimeAndDistance_intersectionDot();

	// void						_init_polyline_formJSON(JSON json);

	std::vector<ObjPolygon *>	_getPolygon_from_db_PGresult(PGresult *res, int i);
	void						_addPolygonTo_db(JSON &json, ObjPolygon *polygon);
	ObjPolygon					*_getPolygonIntoJSON(JSON &json_polygon, int index_arr_dot);
	PGconn						*_connectTo_db();

	static IntersectionType		_intersectionPolygons(ObjPolygon *check_polygon, ObjPolygon *polygon_from_tt);
	static IntersectionType		_checkPartsOfOneArya_and_link_otherParts(
									std::vector<ObjPolygon *> parts_of_one_area);

	void						_addErrorMassage(
									ObjPolygon *polygon,
									std::vector<ObjPolygon *> conflictPolygon,
									IntersectionType typeError);


	std::string					_createNewSQL_forAddPolygonInto_db(
									JSON &json,
									ObjPolygon *polygon);



	static std::vector<ObjPolygon *>	_joinList(
											std::vector<ObjPolygon *> list_p1,
											std::vector<ObjPolygon *> list_p2);
	std::vector<Dot>					_get_arr_dot_fromPolygon_string(
											std::string str_arr);
	void								_add_max_min_center_XY_toPolygonData(
											std::vector<Dot> &arr_dot,
											t_polygon_data &data);
};

#endif
