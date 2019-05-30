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

	void								deletePolygonFrom_db(std::string id);

	Plast								*get_plast();
	ResultStatus						get_status();
	JSON								get_result();

	

private:
	JSON						_result;
	ObjPolyline					*_polyline;
	Plast						*_plast;
	ResultStatus				_status;
	std::vector<ObjPolygon *>	_all_polygons;

	void						_addPolygonTo_controller(std::vector<ObjPolygon *> parts_of_one_area);
	std::vector<ObjPolygon *>	_getPolygon_from_db_PGresult(PGresult *res, int i);
	void						_addPolygonTo_db(JSON &json, ObjPolygon *polygon);
	std::vector<ObjPolygon *>	_getPolygonIntoJSON(JSON &json_polygon);
	PGconn						*_connectTo_db();

	static IntersectionType		_intersectionPolygons(ObjPolygon *check_polygon, ObjPolygon *polygon_from_tt);
	static IntersectionType		_checkPartsOfOneArya_and_link_otherParts(
									std::vector<ObjPolygon *> parts_of_one_area);


	void						_addResult();
	void						_addResult_error(
									ObjPolygon *polygon,
									std::vector<ObjPolygon *> conflictPolygons,
									IntersectionType typeError);
	void						_addResult_error_intersect_fullMatch(
									ObjPolygon *polygon,
									std::vector<ObjPolygon *> conflictPolygons);
	void						_addResult_error_intersect_errorPartsOfOneArya(
									std::vector<ObjPolygon *> parts_of_one_area);


	std::string					_createNewSQL_forAddPolygonInto_db(
									JSON &json,
									ObjPolygon *polygon);
	bool						_checkExistPolygon(std::string id);



	std::vector<ObjPolygon *>			_delete_from_controller_and_return_polygon(
											ObjPolygon	*exist_polygon);

	static std::vector<ObjPolygon *>	_joinList(
											std::vector<ObjPolygon *> list_p1,
											std::vector<ObjPolygon *> list_p2);
	std::vector<Dot>					_get_arr_dot_fromPolygon_string(
											std::string str_arr);
	void								_add_max_min_center_XY_toPolygonData(
											std::vector<Dot> &arr_dot,
											t_polygon_data &data);
	void								_showSituationWithPolygons(std::vector<Plast *> list_plast);
	ObjPolygon							*_getPolygon_by_id(std::string id);
	void								_delete_from_all_polygons_by_id(std::string id);
	void								_add_to_all_polygons(ObjPolygon *polygon);

};

#endif
