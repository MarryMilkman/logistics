#include "LogisticsController.hpp"

#include "ObjPolygon.hpp"
#include "ObjPolyline.hpp"
#include "DotPolyline.hpp"

#include "TetraTreePolygons.hpp"
#include "Plast.hpp"

#include "geometry/Geometry.hpp"
#include "geometry/Line.hpp"

LogisticsController::LogisticsController() {
	this->_plast = new Plast();
	this->_polyline = new ObjPolyline();
}

LogisticsController::~LogisticsController() {
	if (this->_polyline)
		delete this->_polyline;
	if (this->_plast)
		delete this->_plast;
}

LogisticsController	&LogisticsController::getInstance() {
	static LogisticsController	controller;

	return controller;
}




//MARK: - init Polyline

	// init polyline. pars from JSON and init polyline data
void		LogisticsController::init_polyline(JSON &json) {
	
	this->_polyline->free_polyline();
	this->_polyline->init_formJSON(json);

	this->_polyline->analyze_and_update_data();
	// this->_polyline->showDots();
	this->_addResult();
}




// MARK: - LOAD from/to data base
// 1) addPolygon_to_controller_and_db
// 2) reloadPolygonsFrom_db
// 3) _getPolygon_from_db_PGresult
// 4) _addPolygonTo_db
// 5) _getPolygonIntoJSON
// 6) _connectTo_db

	// add polygon to db and controller
void				LogisticsController::addPolygon_to_controller_and_db(
						JSON json_polygon)
{
	std::vector<ObjPolygon *>	parts_of_one_area;
	// ObjPolygon					*polygon;
	IntersectionType			answer;
	std::vector<ObjPolygon *>	conflictPolygons;
	int							count_arr_dot = json_polygon["coords"].size();
	int							i = -1;

	std::cerr << "LogisticsController::addPolygon_to_controller_and_db\n";

	this->_result = {}; // JSON
	parts_of_one_area = _getPolygonIntoJSON(json_polygon);
	for (ObjPolygon * polygon : parts_of_one_area) {
		conflictPolygons = this->_plast->addNewPolygon_toPlast(polygon, 0, answer);
		if (conflictPolygons.size()) {
			// change status
			this->_addResult_error(polygon, conflictPolygons, answer);
			return ;
		}
	}
	answer = LogisticsController::_checkPartsOfOneArya_and_link_otherParts(parts_of_one_area);
	if (answer == IntersectionType::ErrorPartsOfOneArya) {
		this->_addResult_error(0, parts_of_one_area, IntersectionType::ErrorPartsOfOneArya);
		return ;
	}
	this->_addPolygonTo_db(json_polygon["coords"], parts_of_one_area[0]);
	this->_status = ResultStatus::Polygon_added;
}

	// load polygons from data base
void				LogisticsController::reloadPolygonsFrom_db() {
	PGconn						*conn = this->_connectTo_db();
	PGresult					*res;
	std::vector<ObjPolygon *>	parts_of_one_area;	
	ObjPolygon					*polygon;
	std::vector<ObjPolygon *>	conflictPolygons;

	std::cerr << "LogisticsController::reloadPolygonsFrom_db\n";
	this->_status = ResultStatus::Polygon_added;
	if (!conn) {
		std::cerr << "Connect error to db\n";
		exit(1);
	}
	PQsendQuery(conn, "SELECT arr_dot, color, id_polygon FROM table_polygons;");
	while (res = PQgetResult(conn) ) {
		if (PQresultStatus(res) == PGRES_FATAL_ERROR){
			std::cerr<< PQresultErrorMessage(res)<<std::endl;
			exit(0);
		}
		int		nbrRow = PQntuples(res);
		int		i = -1;	

		std::cerr << "find " << nbrRow << " polygons in db\n\n\n\n\n\n";
		while (++i < nbrRow) {
			IntersectionType	answer;

			parts_of_one_area = this->_getPolygon_from_db_PGresult(res, i);
			for (ObjPolygon * polygon : parts_of_one_area) {
				conflictPolygons = this->_plast->addNewPolygon_toPlast(polygon, 0, answer);
				if (conflictPolygons.size()) {
					std::cerr << "ERROR go to error massage\n";
					this->_addResult_error(polygon, conflictPolygons, answer);
					continue ;
				}
				polygon->get_log();
				std::cerr << "\n\n\n";
			}
			//
			answer = LogisticsController::_checkPartsOfOneArya_and_link_otherParts(parts_of_one_area);
			if (answer == IntersectionType::ErrorPartsOfOneArya)
				this->_addResult_error(0, parts_of_one_area, IntersectionType::ErrorPartsOfOneArya);
		}
		PQclear(res);
	}
	if (this->_status != ResultStatus::Polygon_added)
		std::cerr << "ERROR in db:\n" << this->_result.dump(2);
}

	// create new polygon and return it
std::vector<ObjPolygon *>	LogisticsController::_getPolygon_from_db_PGresult(
								PGresult *res,
								int line)
{
	std::vector<ObjPolygon *>	r_parts;
	JSON						json_db;
	int							size;
	int							i = -1;

	std::cerr << "LogisticsController::_getPolygon_from_db_PGresult\n";

	json_db = JSON::parse(std::string(PQgetvalue(res, line, 0))); // [arr_dot]
	size = json_db.size();
	while (++i < size) {
		t_polygon_data				data;
		ObjPolygon					*polygon;
		std::string					strJSON;

		strJSON = json_db[i].dump();
		data.arr_dot = this->_get_arr_dot_fromPolygon_string(strJSON); // arr_dot
		data.color = std::string(PQgetvalue(res, line, 1)); // colot
		data.id = std::string(PQgetvalue(res, line, 2)); // id
		this->_add_max_min_center_XY_toPolygonData(data.arr_dot, data);
		polygon = new ObjPolygon(data);
		r_parts.insert(r_parts.end(), polygon);
	}
	return r_parts;
}

	// add polygon to dt (create sql and exec)
void				LogisticsController::_addPolygonTo_db(JSON &json, ObjPolygon *polygon) {
	std::string		sql_str;
	PGconn			*conn = this->_connectTo_db();

	std::cerr << "LogisticsController::_addPolygonTo_db\n";
	sql_str = this->_createNewSQL_forAddPolygonInto_db(json, polygon);
	PQsendQuery(conn, sql_str.c_str());
	PGresult				*res;

	res = PQgetResult(conn);
	if (res && PQresultStatus(res) == PGRES_FATAL_ERROR){
		std::cerr<< PQresultErrorMessage(res)<<std::endl;
	}
	// sql = sql + "INSERT INTO PeopleTable (id, arr_dot, color) VALUES";

}

	// get std::vector<ObjPolygon *> into json_polygon
std::vector<ObjPolygon *>		LogisticsController::_getPolygonIntoJSON(JSON &json_polygon) {
	std::vector<ObjPolygon *>	parts_of_one_area;
	t_polygon_data				data;
	std::string					string_arr;
	int							nbr_parts;
	int							i = -1;

	std::cerr << "LogisticsController::_getPolygonIntoJSON\n";
	nbr_parts = json_polygon["coords"].size();
	while (++i < nbr_parts) {
		data.id = json_polygon["name"].get<std::string>(); // id
		string_arr = json_polygon["coords"][i].dump();
		data.arr_dot = this->_get_arr_dot_fromPolygon_string(string_arr); // arr_dot
		this->_add_max_min_center_XY_toPolygonData(data.arr_dot, data); // max min XY center
		data.color = json_polygon["color"]; // colot
		parts_of_one_area.push_back(new ObjPolygon(data));
	}
	return parts_of_one_area;
}

	//connect ro db
PGconn							*LogisticsController::_connectTo_db() {
	PGconn	*conn = 0;
	std::string	user = "postgres";
	std::string	password = "1234";
	std::string	host = "localhost";
	std::string	dbname = "polygons";
	std::string	port = "5432";

	std::cerr << "LogisticsController::_connectTo_db\n";

	conn = PQsetdbLogin(host.c_str(), port.c_str(), 0, 0, dbname.c_str(), user.c_str(), password.c_str());
	if (PQstatus(conn) != CONNECTION_OK) {
		std::cerr << "Error in connection to server\n";
		return 0;
	}
	std::cout << "Success!\n";
	return conn;
}




// MARK: -  function for check Intersection check_polygon with ather arya in TetraTreePolygons
// 1) checkIntersection (static)
// 2) _intersectionPolygons (static)
// 3) _checkPartsOfOneArya_and_link_otherParts (static)

	// return std::vector<ObjPolygon *> - pull of polygons,
	// which have some relationship with check ObjPolygon (intersection||include||upper||full match)
	// if relationship were not found, then return empty std::vector
std::vector<ObjPolygon *>	LogisticsController::checkIntersection(
										ObjPolygon *check_polygon,
										TetraTreePolygons *check_tt_polygon,
										IntersectionType *answer)
{
	IntersectionType					answ_from_intersection;
	std::vector<ObjPolygon *>			answerPolygons;
	std::vector<TetraTreePolygons *>	sortList_tt_polygons;
	ObjPolygon							*polygon_from_check_tt_polygon = check_tt_polygon->polygon;

	std::cerr << "LogisticsController::checkIntersection\n";
	if (polygon_from_check_tt_polygon) {
		std::cerr << check_polygon 					<< " : " <<  check_polygon->data.id					<< " - check_polygon\n";
		std::cerr << polygon_from_check_tt_polygon	<< " : " <<  polygon_from_check_tt_polygon->data.id	<< " - polygon_from_check_tt_polygon\n";
	}
	*answer = IntersectionType::Intersection_false;
	//
	answ_from_intersection = LogisticsController::_intersectionPolygons(check_polygon, polygon_from_check_tt_polygon);
	if (answ_from_intersection == IntersectionType::FullMatch) {
		*answer = answ_from_intersection;
		std::vector<ObjPolygon *>	r_vector;

		r_vector.insert(r_vector.end(), polygon_from_check_tt_polygon);
		return r_vector;
	}
	if (answ_from_intersection == IntersectionType::PolygonInclude) {
		*answer = answ_from_intersection;
		std::vector<ObjPolygon *>	r_vector;

		r_vector.insert(r_vector.end(), polygon_from_check_tt_polygon);
		return r_vector;
	}
	if (answ_from_intersection == IntersectionType::Intersection_true) {
		*answer = answ_from_intersection;
		answerPolygons.insert(answerPolygons.end(), polygon_from_check_tt_polygon);
	}
	if (answ_from_intersection == IntersectionType::PolygonUpper && *answer != IntersectionType::Intersection_true) {
		*answer = answ_from_intersection;
		answerPolygons.insert(answerPolygons.end(), polygon_from_check_tt_polygon);
	}
	// Can be ERROR!
	sortList_tt_polygons = TetraTreePolygons::createSortListFromChildOf_tt_polygons_withDestination(check_tt_polygon, check_polygon->data.center);
	int	i;
	for (TetraTreePolygons *new_check_tt_polygons : sortList_tt_polygons) {
		if (!new_check_tt_polygons)
			continue;
		std::vector<ObjPolygon *>	new_answerPolygons;

		new_answerPolygons = LogisticsController::checkIntersection(check_polygon, new_check_tt_polygons, answer);
		if (*answer == IntersectionType::FullMatch) {
			return new_answerPolygons;
		}
		if (*answer == IntersectionType::Intersection_true) {
			if (answ_from_intersection != IntersectionType::Intersection_true)
				answerPolygons = std::vector<ObjPolygon *>();
			answ_from_intersection = *answer;
			//
			answerPolygons = LogisticsController::_joinList(answerPolygons, new_answerPolygons);
		}
		if (*answer == IntersectionType::PolygonUpper && *answer != IntersectionType::Intersection_true) {
			answ_from_intersection = *answer;
			answerPolygons = LogisticsController::_joinList(answerPolygons, new_answerPolygons);
		}
		if (*answer == IntersectionType::PolygonInclude)
			return new_answerPolygons;
	}
	return answerPolygons;
}

	// _intersectionPolygons - check relationships with tow polygons
	// return answer in relation to check_polygon
IntersectionType		LogisticsController::_intersectionPolygons(
											ObjPolygon *check_polygon,
											ObjPolygon *polygon_from_tt)
{
	IntersectionType	status = IntersectionType::FullMatch;
	std::vector<Dot>	&arr_dot = check_polygon->data.arr_dot;

	std::cerr << "LogisticsController::_intersectionPolygons\n";
	if (!ObjPolygon::is_squareOfPolygonsIntersect(check_polygon, polygon_from_tt)) {
		return IntersectionType::Intersection_false;
	}

	for (Dot dot : arr_dot) {
		
		RelativePosition answer = Geometry::whereIs_dotInPolygon(dot, polygon_from_tt);
		// 0 - out of polygon; 1 - in the border; 2 - inside
		if (answer == RelativePosition::Outside) {
			if (status == IntersectionType::PolygonInclude)
				return IntersectionType::Intersection_true;
			status = IntersectionType::Intersection_false;
		}
		if (answer == RelativePosition::Inside) {
			if (status == IntersectionType::Intersection_false)
				return IntersectionType::Intersection_true;
			status = IntersectionType::PolygonInclude;
		}
	}
	if (status == IntersectionType::FullMatch)
		return status;
	if (status == IntersectionType::PolygonInclude)
		return status;
	bool	is_equal = false;

	ObjPolygon	*dominantPolygon = ObjPolygon::checkDominationSquare(check_polygon, polygon_from_tt, is_equal);
	if (is_equal)
		dominantPolygon = check_polygon;
	if (dominantPolygon != check_polygon) {
		return IntersectionType::Intersection_false;
	}
	ObjPolygon	*obedientPolygon = polygon_from_tt;

	Dot		dot = obedientPolygon->data.arr_dot[0];

	if (Geometry::is_dotInPolygon(dot, dominantPolygon))
			return IntersectionType::PolygonUpper;
	return IntersectionType::Intersection_false;
}

	// if Arya inculde many parts, check them parent: if have different - ErrorPartsOfOneArya
IntersectionType				LogisticsController::_checkPartsOfOneArya_and_link_otherParts(
									std::vector<ObjPolygon *> parts_of_one_area)
{
	for (ObjPolygon *polygon : parts_of_one_area) {
		for (ObjPolygon *verified_polygon : parts_of_one_area) {
			if (verified_polygon == polygon)
				continue;
			if (verified_polygon->parent != polygon->parent)
				return IntersectionType::ErrorPartsOfOneArya;
		}
	}
	for (ObjPolygon *polygon : parts_of_one_area) {
		for (ObjPolygon *verified_polygon : parts_of_one_area) {
			if (verified_polygon == polygon)
				continue;
			polygon->otherParts.insert(polygon->otherParts.end(), verified_polygon);
		}
	}
	return IntersectionType::Intersection_false;
}




// MARK: - result hendler

	//add result of polyline
void							LogisticsController::_addResult() {
	JSON	part_result;
	int		i;

	part_result = {
		{
			// coord_x (latitude)
			// coord_y (longitude)
			// distance
			// time
			// id_dot
		},
		{
			{"location", ""},
			{"is_intersect", 0},
			{"next_location", ""}
		}
	};
	i = 0;
	for (DotPolyline * pDot : this->_polyline->list_pDot) {
		part_result[0][0] = pDot->dot.x;		// coord_x (latitude)
		part_result[0][1] = pDot->dot.y;		// coord_y (longitude)
		part_result[0][2] = pDot->distance;		// distance
		part_result[0][3] = pDot->current_time;	// time
		part_result[0][4] = pDot->id;			// id_dot

		part_result[1]["is_intersect"] = pDot->isIntersect ? 1 : 0;
		if (pDot->isIntersect)
			part_result[1]["location"] = pDot->previousArea == 0 ? "" : pDot->previousArea->data.id;
		else
			part_result[1]["location"] = pDot->listOf_contactAreas[0] == 0 ? "" : pDot->listOf_contactAreas[0]->data.id;
		part_result[1]["next_location"] = pDot->nextArea == 0 ? "" : pDot->nextArea->data.id;
		this->_result[i++] = part_result;
	}
}

	//add error reuslt
void							LogisticsController::_addResult_error(
									ObjPolygon *polygon,
									std::vector<ObjPolygon *> conflictPolygons,
									IntersectionType typeError)
{
	std::cerr << "LogisticsController::_addResult_error\n";

	if (typeError == IntersectionType::Intersection_true
		|| typeError == IntersectionType::FullMatch)
		this->_addResult_error_intersect_fullMatch(polygon, conflictPolygons);
	if (typeError == IntersectionType::ErrorPartsOfOneArya)
		this->_addResult_error_intersect_errorPartsOfOneArya(conflictPolygons);
	this->_status = ResultStatus::Polygon_intersect;
}

	// _addResult_error_intersect_fullMatch
void							LogisticsController::_addResult_error_intersect_fullMatch(
									ObjPolygon *polygon,
									std::vector<ObjPolygon *> conflictPolygons)
{
	int			i;
	JSON		part_result = {
		{"conflict", {""}},
		{"name", ""}
	};
	
	i = 0;
	for (ObjPolygon *c_polygon : conflictPolygons) {
		std::cerr << "sasat" << i << "\n\n";
		part_result["conflict"][i++] = c_polygon->data.id;
	}
	if (polygon)
		part_result["name"] = polygon->data.id;
	i = this->_result.size();
	this->_result[i] = part_result;
}

	// _addResult_error_intersect_errorPartsOfOneArya
void							LogisticsController::_addResult_error_intersect_errorPartsOfOneArya(
									std::vector<ObjPolygon *> parts_of_one_area)
{
	int			i;
	JSON		part_result = {
		{"parents", {}},
		{"name", ""}
	};
	
	if (!parts_of_one_area.size())
		return ;
	part_result["name"] = parts_of_one_area[0]->data.id;
	for (ObjPolygon *c_polygon : parts_of_one_area) {
		if (c_polygon->parent)
			part_result["parents"][i++] = c_polygon->parent->data.id;
		else
			part_result["parents"][i++] = "";
	}
	i = this->_result.size();
	this->_result[i] = part_result;
}




// MARK: - work with SQL
// 1) _createNewSQL_forAddPolygonInto_db
// 2) deletePolygonFrom_db

	// ctreate sql for add data to db
std::string			LogisticsController::_createNewSQL_forAddPolygonInto_db(JSON &json, ObjPolygon *polygon) {
	std::string		fits_purt;
	std::string		values_id;
	std::string		values_arr_dot;
	std::string		values_color;
	std::string		r_sql;

	std::cerr << "LogisticsController::_createNewSQL_forAddPolygonInto_db\n";

	fits_purt = "INSERT INTO table_polygons (arr_dot, color, id_polygon) VALUES (";
	
	std::string	str = json.dump();
	values_arr_dot = "\'" + json.dump() + "\', ";
	values_color = "\'"+ polygon->data.color + "\', ";
	values_id = "\'" + polygon->data.id + "\'";
	r_sql = fits_purt + values_arr_dot + values_color + values_id + ");";
	polygon->get_log();
	return r_sql;
}

	// delete polygon from db (create sql and exec)
void							LogisticsController::deletePolygonFrom_db(int id) {
	std::string		sql_str;
	PGconn			*conn = this->_connectTo_db();
	PGresult		*res;

	sql_str = "DELETE FROM table_polygons WHERE id_polygon = \'" + std::to_string(id) + "\'";
	PQsendQuery(conn, sql_str.c_str());
	res = PQgetResult(conn);
	if (res && PQresultStatus(res) == PGRES_FATAL_ERROR) {
		std::cerr<< PQresultErrorMessage(res) << std::endl;
	}
}




// MARK: - create JSON result



// MARK: - useful function
// 1) _joinList (static)
// 2) _get_arr_dot_fromPolygon_string
// 3) _add_max_min_center_XY_toPolygonData
// 4) showSituationWithPolygons
// 5) get_plast

	// join 2 std::vector<ObjPolygon *>
std::vector<ObjPolygon *>		LogisticsController::_joinList(std::vector<ObjPolygon *> list_p1, std::vector<ObjPolygon *> list_p2)
{
	std::vector<ObjPolygon *> r_list;

	std::cerr << "LogisticsController::_joinList\n";
	for (ObjPolygon * p : list_p1)
		r_list.insert(r_list.end(), p);
	for (ObjPolygon * p : list_p2)
		r_list.insert(r_list.end(), p);
	return r_list;
}

	// get array dot from polygonJSON["coord"][0] (std::string)
std::vector<Dot>		LogisticsController::_get_arr_dot_fromPolygon_string(
							std::string str_arr)
{
	std::vector<Dot>	arr_dot;
	JSON				json;
	int					size;
	int					i = -1;

	std::cerr << "LogisticsController::_get_arr_dot_fromPolygon_string\n";
	json = JSON::parse(str_arr);
	size = json.size();
	while (++i < size) {
		Dot		dot(json[i][0].get<double>(), json[i][1].get<double>());

		arr_dot.insert(arr_dot.end(), dot);
	}
	return arr_dot;
}

	// calculate and add max/min coord + center to  t_polygon_data data
void					LogisticsController::_add_max_min_center_XY_toPolygonData(
											std::vector<Dot> &arr_dot,
											t_polygon_data &data)
{
	std::cerr << "LogisticsController::_add_max_min_center_XY_toPolygonData\n";

	data.max_x = -10000;
	data.max_y = -10000;
	data.min_x = 10000;
	data.min_y = 10000;
	for (Dot dot : arr_dot) {
		if (dot.x > data.max_x)
			data.max_x = dot.x;
		if (dot.x < data.min_x)
			data.min_x = dot.x;
		if (dot.y > data.max_y)
			data.max_y = dot.y;
		if (dot.y < data.min_y)
			data.min_y = dot.y;
	}
	data.center.x = (data.max_x + data.min_x ) / 2;
	data.center.y = (data.max_y + data.min_y ) / 2;
}

	// show some sam info about polygons in controller
void							LogisticsController::showSituationWithPolygons() {
	TetraTreePolygons	*tt_polygons;

	if (this->_plast) {
		std::cerr << "polygons in controller plast:\n";
		tt_polygons = this->_plast->_tt_polygons;
		std::cerr << tt_polygons->polygon->data.id << "\n";
		if (tt_polygons->moreX_lessY)
			std::cerr << tt_polygons->moreX_lessY->polygon->data.id << " moreX_lessY\n";
		if (tt_polygons->moreX_moreY)
			std::cerr << tt_polygons->moreX_moreY->polygon->data.id << " moreX_moreY\n";
		if (tt_polygons->lessX_moreY)
			std::cerr << tt_polygons->lessX_moreY->polygon->data.id << " lessX_moreY\n";
		if (tt_polygons->lessX_lessY)
			std::cerr << tt_polygons->lessX_lessY->polygon->data.id << " lessX_lessY\n";
	}
}

Plast					*LogisticsController::get_plast() {
	return this->_plast;
}

ResultStatus			LogisticsController::get_status() {
	return this->_status;
}

JSON					LogisticsController::get_result() {
	return this->_result;
}